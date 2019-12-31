#ifndef SPRITE_H
#define SPRITE_H


void SpriteSetClipWindow(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom);

void enable_sprites();
void set_sprite_pattern_index(uint8_t slot);

void set_pattern(uint8_t* sprite_pattern);

void set_sprite(uint16_t xpos, uint8_t ypos, uint8_t spritePattern, bool visible);
void set_sprite_composite(uint16_t xpos, uint8_t ypos, uint8_t spritePattern);

void create_sprite_patterns();
//bool sprites_load_ball_patterns();

#endif