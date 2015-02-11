#include <cstring>
#include <iostream>
#include <fstream>
#include "xmlparser.h"
#include "rapidxml_print.h"

XMLParser& XMLParser::getInstance() {
    static XMLParser instance;
    return instance;
}

XMLParser::~XMLParser() {
    while(!docs.empty())
     {
         delete docs.begin()->second.first;
         docs.erase(docs.begin());
     }
}

bool XMLParser::parse(const std::string& fn) {

    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::map<std::string, std::pair<rapidxml::xml_document<>*, std::vector<char> > >::const_iterator it;


    // We've already parsed in this document
    if( (it=docs.find(fn)) !=docs.end())
    {  
        curDoc=it->second.first; 
        return true;
    }
    else {

        // rapidxml docs store operates off references to their input vectors, so we need to store 
        // them together.
        try { 
            file.open(fn.c_str(), std::ifstream::in);
        
            std::vector<char> buf((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
            buf.push_back('\0');
            docs.insert(std::pair<std::string, std::pair<rapidxml::xml_document<>*, std::vector<char> > >(fn, std::pair<rapidxml::xml_document<>*, std::vector<char> >(new rapidxml::xml_document<>(), buf)));
        
            file.close();
        }
        catch(std::ifstream::failure e)
        {
            std::cerr << "Error in XMLParser::parse(const std::string& fn) trying to open file \"" << fn << "\"" << std::endl;
            return false;
        }

        // parse the vector and then store it in our doc map
        try { docs.find(fn)->second.first->parse<0>(&((docs.find(fn)->second.second[0]))); }
        catch(rapidxml::parse_error e)
        {
            std::cerr << "Parse error in XMLParser::parse(const std::string& fn) for file " << fn << std::endl;
            std::cerr << e.what() << std::endl;
            return false;
        }

        // finally set the current document to the one we just parsed in
        curDoc = docs.find(fn)->second.first;

        return true;
    }
}

bool XMLParser::setCurDocument(const std::string& fn) {

    std::map<std::string, std::pair<rapidxml::xml_document<>*, std::vector<char> > >::iterator docPair;

    // if we don't have the document yet, try to parse it in
    if((docPair = docs.find(fn)) == docs.end())
    {
        if(parse(fn) == false) return false;
        else docPair = docs.find(fn); 
    }

    // check if we're already set to this;
    else if(curDoc == ((docPair)->second.first)) return true;

    else curDoc = (docPair->second.first);

    return true;
}

void XMLParser::removeDoc(const std::string& fn) {
    std::map<std::string, std::pair<rapidxml::xml_document<>*, std::vector<char> > >::iterator docPair; 

    // Check if the document doesn't exist in the first place ( should we do something else to handle this )
    if( (docPair = docs.find(fn)) == docs.end() ) return;

    else {
        delete docPair->second.first;
        docs.erase(docPair);
        return;
    }
}



// walk entire tree for request tag and return associated value
// XXX TODO This should probably just be a recursive call, to support further nesting, right now very specific to original game.xml
const char* XMLParser::find_value(const std::string& tag) const
{
    char* ret_value=NULL;
    rapidxml::xml_node<>* root_node = curDoc->first_node();

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

// search tree for matching nodes, and return list of them
//
// starting from root...
std::list<const rapidxml::xml_node<>* > XMLParser::findNodes(const std::string& tag) const {
    std::list<const rapidxml::xml_node<>*> ret_list=findNodes(tag, curDoc->first_node());
    if(!ret_list.empty()) return ret_list;
    else throw std::string("Parser: couldn't find tag ")+tag+std::string(" in xml");
}
    

// .. and starting from specific node
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

// Return map of attributes for node
std::map<std::string,std::string> XMLParser::parseNode(const rapidxml::xml_node<>* node) const
{
    std::map<std::string,std::string> map;
    for(rapidxml::xml_attribute<>* attr = node->first_attribute();attr;attr=attr->next_attribute())
    {
        map.insert(std::pair<std::string, std::string>(std::string(attr->name()),std::string(attr->value())));
    }
    return map;
}

// Return list of attribute maps for all nodes matching tag
std::list<std::map<std::string,std::string> > XMLParser::parseNodesWithTag(const std::string& tag) const
{
    std::list<std::map<std::string, std::string> > ret_list = parseNodesWithTag(tag,curDoc->first_node());
    return ret_list;
}

// Return list of attribute maps for all nodes matching tag starting from "node"
std::list<std::map<std::string, std::string> > XMLParser::parseNodesWithTag(const std::string& tag, const rapidxml::xml_node<>* node) const
{
    std::list<std::map<std::string,std::string> > ret_list;
    std::list<const rapidxml::xml_node<>* > nodes = findNodes(tag,node);
    for(std::list<const rapidxml::xml_node<>* >::const_iterator iterator= nodes.begin();iterator!=nodes.end(); ++iterator)
    {
        ret_list.push_back(parseNode(*iterator));
    }
    return ret_list;
}
#endif

// Spit out doc
void XMLParser::displayData() const {
    std::cout << *curDoc << std::endl;
}
