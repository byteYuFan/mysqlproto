//
// Created by wangyufan on 2024-01-21.
//

#include "HzMySQLClient.h"

#include "utils/ByteUtil.h"


bool HzMySQLClient::conn() {
    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ == -1) {
        errno_code_ = 1; //socket init error;
        error_message_ = "Error creating socket.";
        return false;
    }
    std::string ipAddress;
    if (!resolveHostname(host_, ipAddress)) {
        errno_code_ = 5;
        error_message_ = "Failed to resolve hostname: ";
        return false;
    }
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_);
    serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());

    if (connect(socket_fd_, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        errno_code_ = 2;
        error_message_ = "Error connecting to MySQL server.";
        close(socket_fd_);
        return false;
    }
    std::vector<char> buffer(1024);
    ssize_t receive_length;
    while ((receive_length = recv(socket_fd_, buffer.data(), 1024, 0)) == -1) {
        if (receive_length == -1) {
            errno_code_ = 3;
            error_message_ = "Error receiving response.";
        }
    }
    auto deal_res = HzMySQLClient::dealHandShakePacket(buffer);
    auto auth_stream = this->produceAuthPacket(deal_res[0], deal_res[1]);
    write(socket_fd_, auth_stream.data(), auth_stream.size());
    memset(buffer.data(), 0, buffer.size());
    while ((receive_length = recv(socket_fd_, buffer.data(), 1024, 0)) == -1) {
        if (receive_length == -1) {
            errno_code_ = 3;
            error_message_ = "Error receiving response.";
        }
    }
    OKPackage ok;
    ok.read(buffer);
    if (ok.getHeader() != 0x00) {
        ErrorPacket e;
        e.read(buffer);
        errno_code_ = 4;
        error_message_ = e.error_message_.data();
        close(socket_fd_);
        return false;
    }
    status_ = true;
    return true;
}

void HzMySQLClient::getErrorInfo() const {
    if (errno_code_ == 0)return;
    printf("errno_code:%d,error_message:%s\n", errno_code_, error_message_.c_str());
}


std::vector<std::vector<char>> HzMySQLClient::dealHandShakePacket(std::vector<char> &mysql_data) {
    HandshakePacket handshakePacket;
    handshakePacket.read(mysql_data);
    version_ = handshakePacket.server_version_.data();
    thread_id_ = handshakePacket.thread_id_;
    std::vector<std::vector<char>> res;
    res.push_back(handshakePacket.seed_);
    res.push_back(handshakePacket.rest_of_scramble_buff_);
    return res;
}

std::vector<char> HzMySQLClient::produceAuthPacket(const std::vector<char> &rand1, const std::vector<char> &rand2) {
    std::vector<char> seed(rand1.size() + rand2.size());
    std::copy(rand1.begin(), rand1.end(), seed.begin());
    std::copy(rand2.begin(), rand2.end(), seed.begin() + rand1.size());
    AuthPacket auth;
    auth.charset_index_ = 0x8;
    auth.packet_id_ = 1;
    auth.client_flags_ = Capabilities::getClientCapabilities();
    auth.max_package_size_ = 1024 * 1024 * 1024;
    auth.user_ = user_;
    auth.database_ = database_;
    std::vector<unsigned char> password_del(password_.size());
    std::string se(seed.begin(), seed.end());
    auto p = scramble411(password_, se);
    auth.password_.clear();
    auth.password_.insert(auth.password_.begin(), p.begin(), p.end());
    std::vector<char> buffer;
    auth.write(buffer);
    return buffer;
}

bool HzMySQLClient::quit() {
    auto quit_data = HzMySQLClient::produceQuitPackage();
    if (send(socket_fd_, quit_data.data(), quit_data.size(), 0) == -1) {
        errno_code_ = 6;
        error_message_ = "quit error";
        close(socket_fd_);
        status_ = false;
        return false;
    }
    return true;
}

std::vector<char> HzMySQLClient::produceQuitPackage() {
    QuitPacket q;
    std::vector<char> res;
    q.write(res);
    return res;
}

std::vector<char> HzMySQLClient::buildQueryPacket(const std::string &query_str) {
    QueryPacket queryPacket(3, query_str);
    std::vector<char> buffer;
    queryPacket.write(buffer);
    std::vector<char> bytes(5 + static_cast<int>(query_str.size()));
    for (int i = 0; i < bytes.size(); i++) {
        bytes[i] = buffer[i];
    }

    return bytes;

}

bool HzMySQLClient::sendQueryPacket(const std::string &query_str) {
    auto send_data = buildQueryPacket(query_str);
    if (send(socket_fd_, send_data.data(), send_data.size(), 0) == -1) {
        errno_code_ = 7;
        error_message_ = "Send query Packet error.";
        return false;
    }
    return true;
}

bool HzMySQLClient::parseResult(std::vector<char> &v_read_data) {
//读取数据

    auto columnCountPacket = processColumnCountPacket(v_read_data);
    auto columnDefinitionPackets = parseColumnDefinition(columnCountPacket.getColumnCount(), v_read_data);
    auto eofPacket = parseEOFPacket(v_read_data);
    auto resultSetRowPackets = getResultSetRows(columnCountPacket.getColumnCount(), v_read_data);
    for (auto &r: resultSetRowPackets) {
        for (int i = 0; i < columnCountPacket.getColumnCount(); i++) {
            r->getColumnValues()->at(i)->push_back('\0');
            this->query_result_.column_data_->push_back(r->getColumnValues()->at(i));
        }
    }
    return true;
}

ColumnCountPacket HzMySQLClient::processColumnCountPacket(std::vector<char> &src) {
    ColumnCountPacket columnCountPacket;
    columnCountPacket.read(src);
    ByteUtil::bytesCut(columnCountPacket, src);
    return columnCountPacket;
}

std::vector<ColumnDefinitionPacket> HzMySQLClient::parseColumnDefinition(int column_count, std::vector<char> &byte) {
    std::vector<ColumnDefinitionPacket> result;
    for (int i = 0; i < column_count; i++) {
        ColumnDefinitionPacket columnDefinitionPacket;
        columnDefinitionPacket.read(byte);
        result.push_back(columnDefinitionPacket);
        this->query_result_.column_definition_->push_back(
                std::make_shared<std::vector<char>>(columnDefinitionPacket.getColumnName()));
        ByteUtil::bytesCut(columnDefinitionPacket, byte);
    }
    return result;
}

EOFPacket HzMySQLClient::parseEOFPacket(std::vector<char> &byte) {
    EOFPacket eofPacket;
    eofPacket.read(byte);
    ByteUtil::bytesCut(eofPacket, byte);
    return eofPacket;
}

std::vector<std::shared_ptr<ResultSetRowPacket>>
HzMySQLClient::getResultSetRows(int column_count, std::vector<char> &byte) {
    EOFPacket eofPacket;
    std::vector<std::shared_ptr<ResultSetRowPacket>> result;
    while (true) {
        eofPacket.read(byte);
        if ((eofPacket.getHeader() & 0xff) == 0xfe) {
            break;
        }
        auto resultSetRowPacket = std::make_shared<ResultSetRowPacket>(column_count);
        resultSetRowPacket->read(byte);
        ByteUtil::bytesCut(resultSetRowPacket.get(), byte);
        result.push_back(resultSetRowPacket);
    }
    return result;

}

bool HzMySQLClient::query(const std::string &query_sql) {
    if (!sendQueryPacket(query_sql)) {
        return false;
    }
    std::vector<char> data(1024 * 16);
    auto length = recv(socket_fd_, data.data(), 1024 * 16, 0);
    std::vector<char> v_read_data(length);
    std::memcpy(v_read_data.data(), data.data(), length);
    // 检测是否出现error包
    if (v_read_data[4] == static_cast<char>(0xff)) {
        ErrorPacket errorPacket;
        errorPacket.read(v_read_data);
        errno_code_ = 11;
        error_message_ = errorPacket.error_message_.data();
        return false;
    }
    return parseResult(v_read_data);
}

