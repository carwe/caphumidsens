/**
 *  BitIO.h
 *  @author   Andi Dittrich <http://andidittrich.de>
 *  @version  1.0
 *  @license  MIT Style X11 License
*/


#include <inttypes.h>

#ifndef BITIO_H_
#define BITIO_H_

// set bit
static inline void BIT_SET(volatile uint8_t *target, uint8_t bit) __attribute__((always_inline));
static inline void BIT_SET(volatile uint8_t *target, uint8_t bit){
  *target |= (1<<bit);
};

// set clear
static inline void BIT_CLEAR(volatile uint8_t *target, uint8_t bit) __attribute__((always_inline));
static inline void BIT_CLEAR(volatile uint8_t *target, uint8_t bit){
  *target &= ~(1<<bit);
};

// bit toogle
static inline void BIT_TOGGLE(volatile uint8_t *target, uint8_t bit) __attribute__((always_inline));
static inline void BIT_TOGGLE(volatile uint8_t *target, uint8_t bit){
  *target ^= (1<<bit);
};

// set bit by boolean
static inline void BIT_BOOL_SET(volatile uint8_t *target, uint8_t bit, bool enable) __attribute__((always_inline));
static inline void BIT_BOOL_SET(volatile uint8_t *target, uint8_t bit, bool enable){
  if (enable){
    BIT_SET(target, bit);
  }else{
    BIT_CLEAR(target, bit);
  }
};

#endif /* BITIO_H_ */
