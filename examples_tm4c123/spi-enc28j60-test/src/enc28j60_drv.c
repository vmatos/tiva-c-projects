
#include <stdbool.h>
#include <stdint.h>
#include "spi_drv.h"
#include "enc28j60_drv.h"


void enc28_Reset(void) {
  enc28_SPISend(0xFF00);
  // TODO: check if clkready
}
