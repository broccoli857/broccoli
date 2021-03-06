/*
 * @Descripttion: 
 * @version: 
 * @Author: cheng
 * @Date: 2022-04-26 20:40:22
 * @LastEditors: cheng
 * @LastEditTime: 2022-04-26 21:42:39
 */
#include "../broccoli/config.h"
#include "../broccoli/log.h"
#include "yaml-cpp/yaml.h"

# if 0

broccoli::ConfigVar<int>::ptr g_int_value_config = 
        broccoli::Config::Lookup("system.port", (int)8080, "system port");

broccoli::ConfigVar<float>::ptr g_int_valueex_config = 
        broccoli::Config::Lookup("system.port", (float)8080, "system port");

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

    YAML::Node root = YAML::LoadFile("/home/cheng/workspace/broccoli/bin/conf/test.yaml");
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
#endif

class Person {
public:
    std::string m_name;
    int m_age = 0;
    bool m_sex = 0;

    std::string toString() const {
        std::stringstream ss;
        ss << "[Person name=" << m_name
            << " age=" << m_age
            << " sex=" << m_sex
            << "]";
        return ss.str();
    }

    bool operator==(const Person& oth) const {
        return m_name == oth.m_name
            && m_age == oth.m_age
            && m_sex == oth.m_sex;
    }
};

namespace broccoli {
    template<>
    class LexicalCast<std::string, Person> {
    public:
        Person operator() (const std::string& v) {
            YAML::Node node = YAML::Load(v);
            Person p;
            p.m_name = node["name"].as<std::string>();
            p.m_age = node["age"].as<int>();
            p.m_sex = node["sex"].as<bool>();
            return p;
        }
    };

    template<>
    class LexicalCast<Person, std::string> {
    public:
        std::string operator() (const Person& p) {
            YAML::Node node;
            node["name"] = p.m_name;
            node["age"] = p.m_age;
            node["sex"] = p.m_sex;
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };
}

broccoli::ConfigVar<Person>::ptr g_person = 
    broccoli::Config::Lookup("class.person", Person(), "system person");

broccoli::ConfigVar<std::map<std::string, Person>>::ptr g_person_map = 
    broccoli::Config::Lookup("class.map", std::map<std::string, Person>(), "system person");

void test_class() {
    BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "before: " << g_person->getValue().toString() << " - " << g_person->toString();

#define XX_PM(g_var, prefix) \
    { \
        auto m = g_person_map->getValue();  \
        for(auto& i : m) {  \
            BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << prefix << ": " << i.first << " - " << i.second.toString();    \
        }   \
        BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << prefix << ": size=" << m.size(); \
    }

    g_person->addListener([](const Person& old_value, const Person& new_value) {
        BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "old_value=" << old_value.toString()
            << " new_value=" << new_value.toString();
    });

    XX_PM(g_person_map, "class.map before");

    YAML::Node root = YAML::LoadFile("/home/cheng/workspace/broccoli/bin/conf/test.yaml");
    broccoli::Config::LoadFromYaml(root);

    BROCCOLI_LOG_INFO(BROCCOLI_LOG_ROOT()) << "after: " << g_person->getValue().toString() << " - " << g_person->toString();

    XX_PM(g_person_map, "class.map after");
}

void test_log() {

    std::cout << "hello world!" << std::endl;
    static broccoli::Logger::ptr system_log = BROCCOLI_LOG_NAME("system");
    BROCCOLI_LOG_INFO(system_log) << "hello system" << std::endl;
    std::cout << broccoli::loggerMgr::GetInstance()->toYamlString() << std::endl;
    YAML::Node root = YAML::LoadFile("/home/cheng/workspace/broccoli/bin/conf/log.yaml");
    broccoli::Config::LoadFromYaml(root);
    std::cout << "========================" << std::endl;
    std::cout << broccoli::loggerMgr::GetInstance()->toYamlString() << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << root << std::endl;
    BROCCOLI_LOG_INFO(system_log) << "hello system" << std::endl;

    system_log->setFormatter("%d - %m%n");
    BROCCOLI_LOG_INFO(system_log) << "hello system" << std::endl;
}

int main(int argc, char** argv)
{
    // test_config();
    // test_class();
    test_log();

    return 0;
}
