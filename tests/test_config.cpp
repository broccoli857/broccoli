#include "../broccoli/config.h"
#include "../broccoli/log.h"
#include "yaml-cpp/yaml.h"

broccoli::ConfigVar<int>::ptr g_int_value_config = 
        broccoli::Config::Lookup("system.port", (int)8080, "system port");

broccoli::ConfigVar<float>::ptr g_float_value_config = 
        broccoli::Config::Lookup("system.value", (float)10.2f, "system value");

broccoli::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config = 
        broccoli::Config::Lookup("system.int_vec", std::vector<int>{1, 2}, "system int vec");

broccoli::ConfigVar<std::list<int>>::ptr g_int_list_value_config = 
        broccoli::Config::Lookup("system.int_list", std::list<int>{1, 2}, "system int list");\

broccoli::ConfigVar<std::set<int>>::ptr g_int_set_value_config = 
        broccoli::Config::Lookup("system.int_set", std::set<int>{1, 2}, "system int set");

broccoli::ConfigVar<std::unordered_set<int>>::ptr g_int_uset_value_config = 
        broccoli::Config::Lookup("system.int_uset", std::unordered_set<int>{1, 2}, "system int uset");

broccoli::ConfigVar<std::map<std::string, int>>::ptr g_int_map_value_config = 
        broccoli::Config::Lookup("system.str_int_map", std::map<std::string, int>{{"k", 2}}, "system int map");

broccoli::ConfigVar<std::unordered_map<std::string, int>>::ptr g_int_umap_value_config = 
        broccoli::Config::Lookup("system.str_int_umap", std::unordered_map<std::string, int>{{"k", 2}}, "system int umap");

void print_yaml(const YAML::Node& node, int level){
    if(node.IsScalar()){
        BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << std::string(level*4, ' ')
            << node.Scalar() << " - " << node.Type() << " - " << level;
    }else if(node.IsNull()){
        BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << std::string(level*4, ' ')
            << "NULL - " << node.Type() << " - " << level;
    }else if(node.IsMap()){
        for(auto it = node.begin(); it != node.end(); ++it){
            BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << std::string(level*4, ' ')
            << it->first << " - " << it->second.Type() << " - " << level;

            print_yaml(it->second, level+1);
        }
    }else if(node.IsSequence()){
        for(size_t i=0; i<node.size(); ++i){
            BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << std::string(level*4, ' ')
            << i << " - " << node[i].Type() << " - " << level;

            print_yaml(node[i], level+1);
        }
    }
}

void test_yaml(){
    YAML::Node root = YAML::LoadFile("/home/cheng/workspace/broccoli/bin/conf/log.yaml");
    print_yaml(root, 0);
}

void test_config(){
    BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "before: " << g_int_value_config->getValue();
    BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "before: " << g_float_value_config->toString();

#define XX(g_var, name, prefix) \
    { \
        auto& v = g_var->getValue(); \
        for(auto& i : v) {  \
            BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << #prefix " " #name " : "<< i;  \
        }   \
        BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << #prefix " " #name " yaml: "<< g_var->toString();  \
    }

#define XX_M(g_var, name, prefix) \
    { \
        auto& v = g_var->getValue(); \
        for(auto& i : v) {  \
            BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << #prefix " " #name " : {"  \
                    << i.first << " - " << i.second << " }";    \
        }   \
        BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << #prefix " " #name " yaml: "<< g_var->toString();  \
    }

    XX(g_int_vec_value_config, int_vec, before);
    XX(g_int_list_value_config, int_list, before);
    XX(g_int_set_value_config, int_set, before);
    XX(g_int_uset_value_config, int_uset, before);
    XX_M(g_int_map_value_config, int_map, before);
    XX_M(g_int_umap_value_config, int_umap, before);

    YAML::Node root = YAML::LoadFile("/home/cheng/workspace/broccoli/bin/conf/log.yaml");
    broccoli::Config::LoadFromYaml(root);

    BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
    BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "after: " << g_float_value_config->toString();

    XX(g_int_vec_value_config, int_vec, after);
    XX(g_int_list_value_config, int_list, after);
    XX(g_int_set_value_config, int_set, after);
    XX(g_int_uset_value_config, int_uset, after);
    XX_M(g_int_map_value_config, int_map, after);
    XX_M(g_int_umap_value_config, int_umap, after);
}

int main(int argc, char** argv)
{
    // test_yaml();

    test_config();

    return 0;
}
