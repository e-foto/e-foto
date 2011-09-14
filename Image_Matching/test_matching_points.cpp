#include <stdio.h>
#include "MatchingPoints.cpp"

int main(void)
{
/*
	//
	// Basic class test
	//
	MatchingPoints mp;

	// Test constructor
	printf("After constructor:\n");
	printMp(&mp);

	printf("After change values:\n");
	mp.left_x = 732.0;
	mp.left_y = 84.0;
	mp.right_x = 1032.0;
	mp.right_y = 82.0;
	printMp(&mp);
*/
/*
	//
	// List test
	//

	MatchingPointsList myList;

	myList.add(16, 17, 732.0, 84.0, 1032.0, 82.0, 9.8);
	myList.add(16, 17, 372.0, 48.0, 1023.0, 44.0, 9.7);
	myList.add(17, 18, 1567.0, 1432.0, 2001.0, 1429.0, 9.1);
	myList.listMp();

	myList.modify(2, 16, 17, 372.0, 49.0, 1023.0, 44.0, 9.7);
	myList.modify(2, 650000.0, 7450000.0, 15.0);
	myList.listMp(2);

//	myList.save((char *)"lista.txt", MatchingPointsList::saveFull);
//	myList.save((char *)"lista.txt", MatchingPointsList::savePairs);
//	myList.save((char *)"lista.txt", MatchingPointsList::saveXYZ);

	myList.remove(2);
	myList.listMp();
*/

	//
	// Load test
	//

	MatchingPointsList myList;
	myList.load((char *)"lista_final.pix",MatchingPointsList::loadPairsHann, false, 16, 17);
//	myList.listMp();
	myList.filterBadPoints2D();
	myList.save((char *)"lista_final_ok.pix", MatchingPointsList::savePairsHann);

	return 1;
}

