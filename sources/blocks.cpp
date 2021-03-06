#include "blocks.hpp"

BlockTextureData BlockTextures[(int)Block::BlocksCount]={
    {TextureName::Air},
    {TextureName::GrassTop, TextureName::GrassBottom, TextureName::Grass},
    {TextureName::GrassBottom},
    {TextureName::DiamondBlock},
    {TextureName::EmeraldBlock},
    {TextureName::GoldBlock},
    {TextureName::IronBlock},
    {TextureName::OakLogTop, TextureName::OakLogTop, TextureName::OakLog},
    {TextureName::OakPlanks},
    {TextureName::OakLeaves},
    {TextureName::DiamondOre},
    {TextureName::EmeraldOre},
    {TextureName::GoldOre},
    {TextureName::IronOre},
    {TextureName::Cobblestone},
    {TextureName::Stone},
    {TextureName::CoalOre},
    {TextureName::Bedrock}
};