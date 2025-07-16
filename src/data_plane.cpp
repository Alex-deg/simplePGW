// #include "data_plane.h"

// data_plane::data_plane(control_plane &control_plane) : _control_plane(control_plane) {}

// void data_plane::handle_uplink(uint32_t dp_teid, Packet &&packet) {
//
//     std::cout << "uplink..." << std::endl;
//     std::cout << "step 1" << std::endl;
//     auto bearer = _control_plane.find_bearer_by_dp_teid(dp_teid);
//     std::cout << "step 2" << std::endl;
//     auto pdn_connection = bearer->get_pdn_connection();
//     std::cout << "step 3" << std::endl;
//     forward_packet_to_apn(pdn_connection.get_apn_gw(), std::move(packet));
//    
//     // - Ue генерирует IP-пакет и передает его в сторону eNodeB
//     // - eNodeB икапсулирует IP-пакет в GTP-U используя TEID, полученный от SGW. Этот TEID указывает
//     // к какому bearer'у принадлежит трафик
//     // - SGW принимает GTP-U пакет, извлекает TEID и определяет какому PGW его направить. При передаче
//     // в сторону PGW SGW инкапсулирует исходный IP-пакет заново, но уже с другим TEID.
//     // - PGW получает GTP-U пакет, использует TEID из заголовка (Data Plane TEID), чтобы:
//     //   1) определить к какому EPS bearer'у и PDN connecion'у относится пакет;
//     //   2) применить соответствующие правила QoS и фильтрации;
//     //   3) провести тарификацию и контроль трафика
//     // - После обработки PGW распаковывает IP-пакет и маршрутизирует его через SGi-
//     // интерфейс в сторону внешней IP-сети, по значению APN с которой связан PDN
//     // connection.
//
// }

// void data_plane::handle_downlink(const boost::asio::ip::address_v4 &ue_ip, Packet &&packet) {
//
//     std::cout << "downlink..." << std::endl;
//
//     auto pdn_connection = _control_plane.find_pdn_by_ip_address(ue_ip);
//     auto bearer = pdn_connection->get_default_bearer();
//
//     forward_packet_to_sgw(pdn_connection->get_sgw_address(), bearer->get_sgw_dp_teid(), std::move(packet));
//
//     // - IP-пакет приходит из внешней сети на SGi-интерфейс PGW.
//     // - PGW по IP-адресу UE (destination адрес) находит PDN-соединение.
//     // - Определяется, к какому bearer’у отнести данный трафик (по TFT).
//     // - Применяются QoS и политики, аналогично uplink-трафику: скорость, DSCP,
//     // ограничения тарифа.
//     // - IP-пакет инкапсулируется в GTP-U, указывая TEID (DP TEID) SGW — это TEID,
//     // ассоциированный с bearer’ом на стороне SGW.
//     // - Пакет отправляется по S5/S8 интерфейсу от PGW к SGW, далее — к eNodeB, и,
//     // наконец, — к UE.
//
// }

#include "data_plane.h"

data_plane::data_plane(control_plane &control_plane) : _control_plane(control_plane) {}

void data_plane::handle_uplink(uint32_t dp_teid, Packet &&packet) {
    auto bearer = _control_plane.find_bearer_by_dp_teid(dp_teid);
    if (!bearer) return;
    auto pdn = bearer->get_pdn_connection();
    if (!pdn) return;
    forward_packet_to_apn(pdn->get_apn_gw(), std::move(packet));
}

void data_plane::handle_downlink(const boost::asio::ip::address_v4 &ue_ip, Packet &&packet) {
    auto pdn = _control_plane.find_pdn_by_ip_address(ue_ip);
    if (!pdn) {
        std::cerr << "PDN не найден!" << std::endl;
        return;
    }

    auto bearer = pdn->get_default_bearer();
    if (!bearer) {
        std::cerr << "Default bearer не найден!" << std::endl;
        return;
    }

    forward_packet_to_sgw(pdn->get_sgw_address(), bearer->get_sgw_dp_teid(), std::move(packet));
}