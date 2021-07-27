#include "AD5761.h"

AD5761::AD5761(SPI_HandleTypeDef* hspi, GPIO_TypeDef* GPIOx, uint16_t cs): hspi(hspi), GPIOx(GPIOx), cs(cs) {}

AD5761::~AD5761() {
	delete[] regBits;
}

void AD5761::AD5761_CS_HIGH(){
	HAL_GPIO_WritePin(GPIOx, cs, GPIO_PIN_SET);
}

void AD5761::AD5761_CS_LOW(){
	HAL_GPIO_WritePin(GPIOx, cs, GPIO_PIN_RESET);
}

uint16_t AD5761::TransferFunction(float voltage, float refVoltage){
	uint16_t codedOutput;

	switch(range){
		case(RANGE_10_10):
				codedOutput = voltage * 65530.0 / (refVoltage * 8.0) + 4.0 * 65530.0 / 8.0;
				break;
		case(RANGE_5_5):
				codedOutput = voltage * 65530.0 / (refVoltage * 4.0) + 2.0 * 65530.0 / 4.0;
				break;
		case(RANGE_3_3):
				codedOutput = voltage * 65530.0 / (refVoltage * 2.4) + 1.2 * 65530.0 / 2.4;
				break;
		case(RANGE_2_5_7_5):
				codedOutput = voltage * 65530.0 / (refVoltage * 4.0) + 1.0 * 65530.0 / 4.0;
				break;
		case(RANGE_0_20):
				codedOutput = voltage * 65530.0 / (refVoltage * 8.0);
				break;
		case(RANGE_0_16):
				codedOutput = voltage * 65530.0 / (refVoltage * 6.4);
				break;
		case(RANGE_0_10):
				codedOutput = voltage * 65530.0 / (refVoltage * 4.0);
				break;
		case(RANGE_0_5):
				codedOutput = voltage * 65530.0 / (refVoltage * 2.0);
				break;
	}

	return codedOutput;
}

void AD5761::Configurate(CV cv, OVR ovr, B2C b2c, ETS ets, PV pv, RA ra, IRO iro){
	range = ra;
	AD5761_Write(CMD_SW_FULL_RESET, 0x0);
	AD5761_Write(CMD_WR_CTRL_REG, 0x0 << 15 | (cv << 9) | (ovr << 8) | (b2c << 7) | (ets << 6) | (iro << 5) | (pv << 3) | ra);
}

void AD5761::AD5761_Write(uint8_t regAddrCmd, uint16_t regData){
	HAL_Delay(5);
	AD5761_CS_LOW();
	HAL_Delay(5);

	regBits[0] = regAddrCmd;
	regBits[1] = (regData & 0xFF00) >> 8;
	regBits[2] = (regData & 0x00FF) >> 0;

	HAL_SPI_Transmit(hspi, regBits, 3, 10);

	while(HAL_SPI_GetState(hspi)!= HAL_SPI_STATE_READY);

	HAL_Delay(5);
	AD5761_CS_HIGH();
	HAL_Delay(5);
}

unsigned char * AD5761::AD5761_Read(uint8_t regAddrCmd){
	AD5761_CS_HIGH();
	AD5761_CS_LOW();

	regBits[0] = regAddrCmd;
	regBits[1] = 0xff;
	regBits[2] = 0xff;

	HAL_SPI_Receive(hspi, regBits, 3, 10);

	return regBits;
}

void AD5761::AD5761_SetVoltage(float voltage, float refVoltage){
	AD5761_Write(CMD_WR_UPDATE_DAC_REG, TransferFunction(voltage, refVoltage));
}
