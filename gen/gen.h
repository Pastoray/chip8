#ifndef GEN_H
#define GEN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct
{
	uint8_t* data;
	size_t size;
	size_t cap;
} ROM;

typedef uint16_t opcode;

static inline opcode clear_screen(void);
static inline opcode return_from_subroutine(void);

static inline opcode jump(uint16_t nnn);
static inline opcode call(uint16_t nnn);

static inline opcode skip_if_vx_eq_kk(uint8_t x, uint8_t kk);
static inline opcode skip_if_vx_neq_kk(uint8_t x, uint8_t kk);

static inline opcode skip_if_vx_eq_vy(uint8_t x, uint8_t y);
static inline opcode skip_if_vx_neq_vy(uint8_t x, uint8_t y);

static inline opcode set_vx_to_kk(uint8_t x, uint8_t kk);
static inline opcode set_vx_to_vx_plus_kk(uint8_t x, uint8_t kk);

static inline opcode set_vx_to_vy(uint8_t x, uint8_t y);
static inline opcode set_vx_to_vx_or_vy(uint8_t x, uint8_t y);
static inline opcode set_vx_to_vx_and_vy(uint8_t x, uint8_t y);
static inline opcode set_vx_to_vx_xor_vy(uint8_t x, uint8_t y);
static inline opcode set_vx_to_vx_plus_vy(uint8_t x, uint8_t y);
static inline opcode set_vx_to_vx_minus_vy(uint8_t x, uint8_t y);

static inline opcode bitshift_right_vx(uint8_t x);
static inline opcode set_vx_to_vy_minus_vx(uint8_t x, uint8_t y);
static inline opcode bitshift_left_vx(uint8_t x);

static inline opcode set_i_to_nnn(uint16_t nnn);
static inline opcode jump_to_nnn_plus_v0(uint16_t nnn);
static inline opcode set_vx_randbyte_and_kk(uint8_t x, uint8_t kk);

static inline opcode display_sprite_with_collision(uint8_t x, uint8_t y, uint8_t n);

static inline opcode skip_if_key_pressed(uint8_t x);
static inline opcode skip_if_not_key_pressed(uint8_t x);

static inline ROM* init_rom(void);
static inline int write_rom(ROM* rom, const char* name);
static inline int write_to_rom(ROM* rom, opcode op);
static inline void free_rom(ROM* rom);

#endif 