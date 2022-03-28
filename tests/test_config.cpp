#include "../broccoli/config.h"
#include "../broccoli/log.h"
#include "yaml-cpp/yaml.h"

broccoli::ConfigVar<int>::ptr g_int_value_config = 
        broccoli::Config::Lookup("system.port", (int)8080, "system port");

broccoli::ConfigVar<float>::ptr g_float_value_config = 
        broccoli::Config::Lookup("system.value", (float)10.2f, "system value");

broccoli::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config = 
        broccoli::Config::Lookup("system.int_vec", std::vector<int>{1, 2}, "system int vec");

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

    auto v = g_int_vec_value_config->getValue();
    for(auto& i : v) {
        BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "before int_vec: " << i;
    }

    YAML::Node root = YAML::LoadFile("/home/cheng/workspace/broccoli/bin/conf/log.yaml");
    broccoli::Config::LoadFromYaml(root);

    BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
    BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "after: " << g_float_value_config->toString();

    v = g_int_vec_value_config->getValue();
    for(auto& i : v) {
        BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "after int_vec: " << i;
    }
}

int main(int argc, char** argv)
{
    // test_yaml();

    test_config();

    return 0;
}
