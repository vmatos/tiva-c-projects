#ifndef _PIN_H_
#define _PIN_H_

#include <stdbool.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

#define PORT_WIDTH 8
#define PORT_COUNT PIN_COUNT/PORT_WIDTH

typedef enum {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7,
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7,
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7,
    PIN_COUNT
} tPin;

typedef struct
{
    unsigned int periph;
    unsigned int base;
}tPort;


static tPort _ports[6] =
{
    {SYSCTL_PERIPH_GPIOA, GPIO_PORTA_BASE},
    {SYSCTL_PERIPH_GPIOB, GPIO_PORTB_BASE},
    {SYSCTL_PERIPH_GPIOC, GPIO_PORTC_BASE},
    {SYSCTL_PERIPH_GPIOD, GPIO_PORTD_BASE},
    {SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE},
    {SYSCTL_PERIPH_GPIOF, GPIO_PORTF_BASE}
};


typedef enum { IN, OUT } PinDir;
typedef enum { LOW, HIGH } PinOutState;

class Pin
{
    public:
        /*typedef enum { OpenCollector, PushPull } PinMode;*/

        Pin( tPin );

        void setDir( PinDir );
        void set( PinOutState );
        void toggle( void );
        unsigned char read( void );

        void enable( void );
        void disable( void );

        // TODO: Make these readonly
        tPort port;
        unsigned char offset;

    private:
        PinDir _dir;
        PinOutState _outState;
};

static Pin* _pins[PIN_COUNT];

#endif
