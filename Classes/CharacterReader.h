//
//  CharacterReader.hpp
//  SushiNeko
//
//  Created by Eiji Ito on 2016/04/26.
//
//

#ifndef CharacterReader_hpp
#define CharacterReader_hpp

#include "cocos2d.h"
#include "cocostudio/WidgetReader/NodeReader/NodeReader.h"

class CharacterReader : public cocostudio::NodeReader
{
public:
    static CharacterReader* getInstance();
    static void purge();
    cocos2d::Node* createNodeWithFlatBuffers(const flatbuffers::Table* nodeOptions);
};


#endif /* CharacterReader_hpp */
