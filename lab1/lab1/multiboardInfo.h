/*
 * multiboardInfo.h
 *
 * Created: 20.10.2017 14:58:48
 *  Author: nicolabc
 */ 


#ifndef MULTIBOARDINFO_H_
#define MULTIBOARDINFO_H_

extern int GAMESTATUS;

/*Reads the data from the multiboard and saves it in the can message struct*/
int multiboardInfo_update(can_msg* yourMessage);

#endif /* MULTIBOARDINFO_H_ */