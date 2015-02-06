/* 
 * File:   i2c_customization.h
 * Author: rene
 *
 * Created on 18. August 2013, 19:30
 */

#ifndef I2C_CUSTOMIZATION_H
#define	I2C_CUSTOMIZATION_H
#define FCY 8000000UL

#define SDA_dir TRISBbits.TRISB2
#define SDA LATBbits.LATB2
#define SDA_Read PORTBbits.RB2
#define SCL_dir TRISBbits.TRISB3
#define SCL LATBbits.LATB3




#endif	/* I2C_CUSTOMIZATION_H */

