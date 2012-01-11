#include "DemFeatures.cpp"
#include "MatchingPoints.cpp"

int main(void)
{
	DemFeatures df;
	MatchingPointsList mpl;

//	printf("Feature type: %s\n",df.getFeatureTypeName(2).c_str());
//	df.loadFeatures((char *)"/home/marts/EFOTO/Develop/data_and_images/UERJ_FEATURES.txt",1);
	df.loadFeatures((char *)"/home/marcelo/projects/e-foto/Old E-FOTO/Database/FEICOES_UERJ_OK_4.txt",1);
//	df.getFeature(8).description = "Rua do bairro do Maracanã"; // Ok, read only
//	df.showFeatures(true);

//	printf("Point id: %d\n",df.addNewPoint(8, 680890.0, 7465159.0, 20.0)); // Ok
//	df.updatePoint(8, 4, 680890.0, 7465159.0, 20.0); // Ok
//	df.setName(8, "Teste"); // Ok
	df.setDescription(8, "Rua do bairro do Maracanã");
//	df.deleteFeature(7); // Ok
//	df.deletePoint(8,7); // Ok
//	df.addNewPoint(8,8,10.0,20.0,30.0); // Ok
//	df.copyFeature(6); // Ok
//	df.addNewPoint(9,2,1.0,2.0,3.0); // Ok, work independently
	df.copyFeature(7,1.0);
	df.showFeatures(true);
//	df.showClasses(); // Ok
//	printf("Nearest feature: %d\n",df.getNearestFeature(681102.3714, 7465262.5574, 21.76));
//	printf("Nearest feature: %d\n",df.getNearestFeature(680828.2934, 7465382.5400, 13.69));

	df.addFeaturesToPairList(&mpl,true);
	mpl.listMp();

	return 1;
}
