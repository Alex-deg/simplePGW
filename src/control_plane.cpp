#include "control_plane.h"

std::shared_ptr<pdn_connection> control_plane::find_pdn_by_cp_teid(uint32_t cp_teid) const {
    auto it = _pdns.find(cp_teid);
    return (it != _pdns.end()) ? it->second : nullptr;
}

std::shared_ptr<pdn_connection> control_plane::find_pdn_by_ip_address(const boost::asio::ip::address_v4 &ip) const {
    auto it = _pdns_by_ue_ip_addr.find(ip);
    return (it != _pdns_by_ue_ip_addr.end()) ? it->second : nullptr;
}

std::shared_ptr<bearer> control_plane::find_bearer_by_dp_teid(uint32_t dp_teid) const {
    auto it = _bearers.find(dp_teid);
    return (it != _bearers.end()) ? it->second : nullptr;
}

std::shared_ptr<pdn_connection> control_plane::create_pdn_connection(const std::string &apn,
                                                                     boost::asio::ip::address_v4 sgw_addr,
                                                                     uint32_t sgw_cp_teid) {
    // ???
    return std::shared_ptr<pdn_connection>(new pdn_connection(sgw_cp_teid, std::move(_apns[apn]), std::move(sgw_addr)));
}

void control_plane::delete_pdn_connection(uint32_t cp_teid) {
    _pdns.erase(cp_teid);
}

std::shared_ptr<bearer> control_plane::create_bearer(const std::shared_ptr<pdn_connection> &pdn, 
                                                           uint32_t sgw_teid) {
    // ???
    return std::shared_ptr<bearer>(new bearer(sgw_teid, *pdn));
}

void control_plane::delete_bearer(uint32_t dp_teid) {
    _bearers.erase(dp_teid);
}

void control_plane::add_apn(std::string apn_name, boost::asio::ip::address_v4 apn_gateway) {
    _apns[apn_name] = apn_gateway;
}
