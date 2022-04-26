/*
 * @Descripttion: 
 * @version: 
 * @Author: cheng
 * @Date: 2022-04-26 21:42:14
 * @LastEditors: cheng
 * @LastEditTime: 2022-04-26 21:42:19
 */
#include "config.h"

namespace broccoli {

static broccoli::Logger::ptr g_logger = BROCCOLI_LOG_NAME("system");

ConfigVarBase::ptr Config::LookupBase(const std::string& name){
    auto it = GetDatas().find(name);
    return it == GetDatas().end() ? nullptr : it->second;
}

static void ListAllMember(const std::string& prefix, const YAML::Node& node, 
                std::list<std::pair<std::string, const YAML::Node>>& output) {
    if(prefix.find_first_not_of("abcdefghigklmnopqrstuvwxyz._0123456789") 
        != std::string::npos) {
        BROCCOLI_LOG_ERROR(BROCCOLI_LOG_ROOT()) << "Config invalid name: " << prefix << " : " << node;
        return;
    }
    output.push_back(std::make_pair(prefix, node));
    if(node.IsMap()) {
        for(auto it=node.begin(); it != node.end(); ++it) {
            ListAllMember(prefix.empty() ? it->first.Scalar() 
                    : prefix + "." + it->first.Scalar(), it->second, output);
        }
    }
}

void Config::LoadFromYaml(const YAML::Node& root) {
    std::list<std::pair<std::string, const YAML::Node>> all_nodes;
    ListAllMember("", root, all_nodes);

    for(auto& i : all_nodes) {
        std::string key = i.first;
        if(key.empty()) {
            continue;
        }

        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        ConfigVarBase::ptr var = LookupBase(key);

        if(var) {
            if(i.second.IsScalar()) {
                var->fromString(i.second.Scalar());
            }else {
                std::stringstream ss;
                ss << i.second;
                var->fromString(ss.str());
            }
        }
    }
}

static std::map<std::string, uint64_t> s_file2modifytime;

void Config::LoadFromConfDir(const std::string& path, bool force) {
    // std::string absoulte_path = broccoli::EnvMgr::GetInstance()->getAbsolutePath(path);
    
}

}
