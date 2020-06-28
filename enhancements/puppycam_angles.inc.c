///This is the bit that defines where the angles happen. They're basically environment boxes that dictate camera behaviour.
//Permaswap is a boolean that simply determines wether or not when the camera changes at this point it stays changed. 0 means it resets when you leave, and 1 means it stays changed.
//The camera position fields accept "32767" as an ignore flag.
struct newcam_hardpos newcam_fixedcam[] =
{

{/*Level ID*/ 16,/*Area ID*/ 1,/*Permaswap*/ 0,/*Mode*/ NC_MODE_FIXED_NOMOVE, //Standard params.
/*X begin*/ -540,/*Y begin*/ 800,/*Z begin*/ -3500, //Where the activation box begins
/*X end*/ 540,/*Y end*/ 2000,/*Z end*/ -1500, //Where the activation box ends.
/*Cam X*/ 0,/*Cam Y*/ 1500,/*Cam Z*/ -1000, //The position the camera gets placed for NC_MODE_FIXED and NC_MODE_FIXED_NOMOVE
/*Look X*/ 0,/*Look Y*/ 800,/*Look Z*/ -2500}, //The position the camera looks at for NC_MODE_FIXED_NOMOVE

};
