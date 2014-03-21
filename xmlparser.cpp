#include <cstring>
#include <iostream>
#include "xmlparser.h"
#include "rapidxml_print.h"

// walk entire tree for request tag and return associated value
const char* XMLParser::find_value(const std::string& tag) const
{
    char* ret_value=NULL;
    rapidxml::xml_node<>* root_node = doc.first_node();

    // start at layer below root, should be tags like <yeti> and <snowball> etc, only handles trees with one layer of nesting (aka game.xml)
    for(rapidxml::xml_node<>*node=root_node->first_node();node;node=node->next_sibling())
    {
        //if it has a value check the name, if match return
        if(node->value())
        {
            if(strcmp(node->name(),tag.c_str())==0)
            {
                ret_value=node->value();
                break;
            }
        }
        // if it has attributes them, walk through them and if match return
        if(node->first_attribute())
        {
            for(rapidxml::xml_attribute<>* attr = node->first_attribute();attr;attr=attr->next_attribute())
            {
                if(strcmp(attr->name(),tag.c_str())==0)
                {
                    ret_value=attr->value();
                    break;
                }
            }
        }
        // now walk through the node's children and do the same
        for(rapidxml::xml_node<>*child_node=node->first_node();child_node;child_node=child_node->next_sibling())
        {
            if(strcmp(child_node->name(),tag.c_str())==0)
            {
                ret_value = child_node->value();
                break;
            }
            if(child_node->first_attribute())
            {
                for(rapidxml::xml_attribute<>* attr = child_node->first_attribute();attr;attr=attr->next_attribute())
                {
                    if(strcmp(attr->name(),tag.c_str())==0)
                    {
                        ret_value=attr->value();
                        break;
                    }   
                }
             }
        }
    }
    if(ret_value) 
    {
        return ret_value;
    }
    else throw std::string("Xml: Didn't find tag ")+tag+std::string(" in xml");
}
#define DEV
#ifdef DEV

std::list<const rapidxml::xml_node<>* > XMLParser::findNodes(const std::string& tag) const {
    std::list<const rapidxml::xml_node<>*> ret_list=findNodes(tag, doc.first_node());
    if(!ret_list.empty()) return ret_list;
    else throw std::string("Parser: couldn't find tag ")+tag+std::string(" in xml");
}
    

// search tree for matching nodes, and return list of them
std::list<const rapidxml::xml_node<>* > XMLParser::findNodes(const std::string& tag, const rapidxml::xml_node<>* n) const {

    std::list<const rapidxml::xml_node<>*> ret_list;
    if(strcmp(n->name(), tag.c_str())==0)
    {
        ret_list.push_back(n);
        return ret_list;
    }

    for(rapidxml::xml_node<>* node = n->first_node();node;node=node->next_sibling())
    {   
        std::list<const rapidxml::xml_node<>*> temp=findNodes(tag,node);
        ret_list.splice(ret_list.end(),temp);
    }
    return ret_list;
}

std::map<std::string,std::string> XMLParser::parseNode(const rapidxml::xml_node<>* node) const
{
    std::map<std::string,std::string> map;
    for(rapidxml::xml_attribute<>* attr = node->first_attribute();attr;attr=attr->next_attribute())
    {
        map.insert(std::pair<std::string, std::string>(std::string(attr->name()),std::string(attr->value())));
    }
    return map;
}



// wwalk through each child, check name, if match return map, then check their children

#endif
void XMLParser::displayData() const {
    std::cout << doc << std::endl;
}
