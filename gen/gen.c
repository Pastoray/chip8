#include "gen.h"

#include <stdint.h>

static inline opcode clear_screen()
{
	return 0x00E0;
}

static inline opcode return_from_subroutine()
{
	return 0x00EE;
}

static inline opcode jump(uint16_t nnn)
{
	return 0x1000 | (nnn & 0x0FFF);
}

static inline opcode call(uint16_t nnn)
{
	return 0x2000 | (nnn & 0x0FFF);
}

static inline opcode skip_if_vx_eq_kk(uint8_t x, uint8_t kk)
{
	return 0x3000 | ((x & 0x0F) << 8) | (kk & 0xFF);
}

static inline opcode skip_if_vx_neq_kk(uint8_t x, uint8_t kk)
{
	return 0x4000 | ((x & 0x0F) << 8) | (kk & 0xFF);
}

static inline opcode skip_if_vx_eq_vy(uint8_t x, uint8_t y)
{
	return 0x5000 | ((x & 0x0F) << 8) | ((y & 0x0F) << 4);
}

static inline opcode set_vx_to_kk(uint8_t x, uint8_t kk)
{
	return 0x6000 | ((x & 0x0F) << 8) | (kk & 0xFF);
}

static inline opcode set_vx_to_vx_plus_kk(uint8_t x, uint8_t kk)
{
	return 0x7000 | ((x & 0x0F) << 8) | (kk & 0xFF);
}

static inline opcode set_vx_to_vy(uint8_t x, uint8_t y)
{
	return 0x8000 | ((x & 0x0F) << 8) | ((y & 0x0F) << 4) | 0x00;
}

static inline opcode set_vx_to_vx_or_vy(uint8_t x, uint8_t y)
{
	return 0x8000 | ((x & 0x0F) << 8) | ((y & 0x0F) << 4) | 0x01;
}

static inline opcode set_vx_to_vx_and_vy(uint8_t x, uint8_t y)
{
	return 0x8000 | ((x & 0x0F) << 8) | ((y & 0x0F) << 4) | 0x02;
}

static inline opcode set_vx_to_vx_xor_vy(uint8_t x, uint8_t y)
{
	return 0x8000 | ((x & 0x0F) << 8) | ((y & 0x0F) << 4) | 0x03;
}

static inline opcode set_vx_to_vx_plus_vy(uint8_t x, uint8_t y)
{
	return 0x8000 | ((x & 0x0F) << 8) | ((y & 0x0F) << 4) | 0x04;
}

static inline opcode set_vx_to_vx_minus_vy(uint8_t x, uint8_t y)
{
	return 0x8000 | ((x & 0x0F) << 8) | ((y & 0x0F) << 4) | 0x05;
}

static inline opcode bitshift_right_vx(uint8_t x)
{
	return 0x8000 | ((x & 0x0F) << 8) | 0x06;
}

static inline opcode set_vx_to_vy_minus_vx(uint8_t x, uint8_t y)
{
	return 0x8000 | ((x & 0x0F) << 8) | ((y & 0x0F) << 4) | 0x07;
}

static inline opcode bitshift_left_vx(uint8_t x)
{
	return 0x8000 | ((x & 0x0F) << 8) | 0x0E;
}

static inline opcode skip_if_vx_neq_vy(uint8_t x, uint8_t y)
{
	return 0x9000 | ((x & 0x0F) << 8) | ((y & 0x0F) << 4);
}

static inline opcode set_i_to_nnn(uint16_t nnn)
{
	return 0xA000 | (nnn & 0x0FFF);
}

static inline opcode jump_to_nnn_plus_v0(uint16_t nnn)
{
	return 0xB000 | (nnn & 0x0FFF);
}

static inline opcode set_vx_randbyte_and_kk(uint8_t x, uint8_t kk)
{
	return 0xC000 | ((x & 0x0F) << 8) | (kk & 0xFF);
}

static inline opcode display_sprite_with_collision(uint8_t x, uint8_t y, uint8_t n)
{
	return 0xD000 | ((x & 0x0F) << 8) | ((y & 0x0F) << 4) | (n & 0x0F);
}

static inline opcode skip_if_key_pressed(uint8_t x)
{
	return 0xE000 | ((x & 0x0F) << 8) | 0x9E;
}

static inline opcode skip_if_not_key_pressed(uint8_t x)
{
	return 0xE000 | ((x & 0x0F) << 8) | 0xA1;
}


static inline ROM* init_rom()
{
	ROM* rom = malloc(sizeof(ROM));
	if (!rom) return NULL;

	rom->capacity = 64;
	rom->size = 0;
	rom->data = malloc(rom->capacity);
	if (!rom->data)
	{
		free(rom);
		return NULL;
	}

	return rom;
}

static inline int write_to_rom(ROM* rom, opcode op)
{
	if (rom->size + 2 > rom->capacity)
	{
		size_t new_capacity = rom->capacity * 2;
		uint8_t* new_data = realloc(rom->data, new_capacity);
		if (!new_data) return -1;

		rom->data = new_data;
		rom->capacity = new_capacity;
	}

	// Write opcode in big endian format
	rom->data[rom->size++] = (op >> 8) & 0xFF;
	rom->data[rom->size++] = op & 0xFF;

	return 0;
}

static inline int write_rom(ROM* rom, const char* name)
{
	char filename[256];
	snprintf(filename, sizeof(filename), "%s.ch8", name);

	FILE* f = fopen(filename, "wb");
	if (!f) return -1;

	size_t written = fwrite(rom->data, 1, rom->size, f);
	fclose(f);

	return (written == rom->size) ? 0 : -1;
}

static inline void free_rom(ROM* rom)
{
	if (!rom) return;
	free(rom->data);
	free(rom);
}