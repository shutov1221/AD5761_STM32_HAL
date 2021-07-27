#include <Registers_AD5761.h>
#include "stm32f4xx_hal.h"

#ifndef AD5761_H_
#define AD5761_H_

class AD5761 {
public:
	AD5761(SPI_HandleTypeDef* hspi, GPIO_TypeDef* GPIOx, uint16_t cs);
	void Configurate(CV cv, OVR ovr, B2C b2c, ETS ets, PV pv, RA ra, IRO iro = IRO_ENABLED);
	void AD5761_Write(uint8_t regAddrCmd, uint16_t regData);
	unsigned char* AD5761_Read(uint8_t regAddrCmd);
	void AD5761_SetVoltage(float voltage, float refVoltage = 2.5);

	virtual ~AD5761();
private:
	uint16_t TransferFunction(float voltage, float refVoltage);
	void AD5761_CS_LOW();
	void AD5761_CS_HIGH();
	unsigned char * regBits = new unsigned char[3];
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* GPIOx;
	uint16_t cs;
	RA range;
};

#endif /* AD5761_H_ */
