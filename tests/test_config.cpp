#include "../broccoli/config.h"
#include "../broccoli/log.h"
#include "yaml-cpp/yaml.h"

broccoli::ConfigVar<int>::ptr g_int_value_config = 
        broccoli::Config::Lookup("system.port", (int)8080, "system port");

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

int main(int argc, char** argv)
{
    BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << g_int_value_config->getValue();
    BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << g_int_value_config->toString();

    test_yaml();

    return 0;
}
