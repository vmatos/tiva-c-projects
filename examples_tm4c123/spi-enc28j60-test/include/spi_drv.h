#ifndef _SPI_DRV_H_
#define _SPI_DRV_H_

void enc28_InitSPI(uint32_t ui32_bitrate, uint32_t ui32_SSIx);

uint16_t enc28_SPISend(uint16_t ui16_rw);

#endif // _SPI_DRV_H_
