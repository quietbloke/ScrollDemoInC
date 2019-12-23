#ifndef SPRITE_H
#define SPRITE_H


void SpriteSetClipWindow(unsigned char left, unsigned char right, unsigned char top, unsigned char bottom);

void enable_sprites();
void set_sprite_pattern_index(uint8_t slot);

void set_pattern(uint8_t* sprite_pattern);

void set_sprite(uint16_t xpos, uint8_t ypos, uint8_t spritePattern);
void set_sprite_composite(uint16_t xpos, uint8_t ypos, uint8_t spritePattern);

bool sprites_load_patterns(char* filename);
bool sprites_load_ball_patterns();

#endif