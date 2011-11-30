#include "ImageMatching.h"
#include "DEMManager.h"

ImageMatching::ImageMatching()
{
        init();
}

ImageMatching::ImageMatching(DEMManager *_man)
{
        init();
        manager = _man;
}

void ImageMatching::init()
{
        matching_method = LSM;
        radiometric_mode = HistMatching;
        perform_readiometric = true;
        image_depth = 256;
        matching_xi = matching_xf = matching_yi = matching_yf = 0;
        step_x = step_y = 3;
        corr_th = 0.7;
        stack = aux = NULL;
        manager = NULL;
        cancel_flag = false;
}

void ImageMatching::setMatchingLimits(int xi, int xf, int yi, int yf)
{
	// Convert C++ vector coordinate system (0 to N-1) to matrix coordinate system (1 to N)
	matching_xi = xi + 1;
	matching_xf = xf + 1;
	matching_yi = yi + 1;
	matching_yf = yf + 1;
}

void ImageMatching::setStep(int sx, int sy)
{
	if (sx < 3) sx = 3;
	if (sy < 3) sy = 3;

	step_x = sx;
	step_y = sy;
}

void ImageMatching::setMinStd(double std)
{
	ncc.setMinStd(std);
	lsm.setMinStd(std);
}

void ImageMatching::performImageMatching(Matrix *img1, Matrix *img2, MatchingPointsList *repository, MatchingPointsList *mpoints)
{
        cancel_flag = false;

	//
	// Step 1 - Radiometric Tranformation
	//

	if (perform_readiometric)
	{
		// Histogram equalization
		if (radiometric_mode == Equalization)
		{
			rt.histeq(img1, image_depth);
			rt.histeq(img2, image_depth);
		}

		// Histogram matching
		if (radiometric_mode == HistMatching)
			rt.histmatching(img1, img2, image_depth);
	}

	//
	// Step 2 - Evaluate and fix matching limits
	//

	int border = 30;
	img_width = img1->getCols();
	img_height = img1->getRows();
	simg_width = img2->getCols();
	simg_height = img2->getRows();
	if (matching_xi < 1 + border || matching_xi > img_width - border)
		matching_xi = 1 + border;

	if (matching_xf < 1 + border || matching_xf > img_width - border)
		matching_xf = img_width - border;

	if (matching_yi < 1 + border || matching_yi > img_height - border)
		matching_yi = 1 + border;

	if (matching_yf < 1 + border || matching_yf > img_height - border)
		matching_yf = img_height - border;

	smatching_xi = 1 + border;
	smatching_xf = simg_width - border;
	smatching_yi = 1 + border;
	smatching_yf =  simg_height - border;

	//
	// Step 3 - Create Region Growing map
	//
	int map_width = img_width/step_x, map_height = img_height/step_y;
	map.resize(map_height, map_width);
        fillMap(mpoints);

	//
	// Step 4 - Read seed repository
	//

	// NOTE: NCC and LSM parameters must be set by the parent class or program

	int num_seeds = repository->size();
	MatchingPoints mp;
	double lx, ly, rx, ry;
	int curr_left_id, curr_right_id;

	// Calculate coverage
	coverage = 0.0;
	max_size = double(map.getCols()*map.getRows());
	num_visited = 0.0;

	for (int i=1; i<=num_seeds; i++)
	{
		// Read current seed, converting to matrix coordinate system
		mp = *repository->get(i);
		lx = mp.left_x + 1;
		ly = mp.left_y + 1;
		rx = mp.right_x + 1;
		ry = mp.right_y + 1;
		curr_left_id = mp.left_image_id;
		curr_right_id = mp.right_image_id;

		//
		// Step 5 - Perform Region Growing
		//

                if (curr_left_id == left_image_id && curr_right_id == right_image_id)
                {
                        region_growing(img1,img2,mpoints,lx,ly,rx,ry);
                        if (cancel_flag)
                            return;
                }
	}

	//
	// Step 6 - Eliminate bad points using statistics
	//

	// Default values: sigma_x=3, sigma_y=1.5
        mpoints->filterBadPoints2D();
}

/*
 * Fill map using matching list, if this is the 2nd or greater running time
 **/
void ImageMatching::fillMap(MatchingPointsList * mpoints)
{
    MatchingPoints *mp;
    int i, j, lx, ly;

    for (int f=1; f<=mpoints->size(); f++)
    {
        mp = mpoints->get(f);

        if (left_image_id != mp->left_image_id || right_image_id != mp->right_image_id)
            continue;

        lx = (int) mp->left_x;
        ly = (int) mp->left_y;

        i = (ly/step_y) + 1;
        j = (lx/step_x) + 1;

        map.set(i,j,1.0);
    }
}

/****************************
 * Region Growing functions *
 ****************************/ 

// Adapted from:
// http://en.wikipedia.org/wiki/Flood_fill
// http://www.student.kuleuven.be/~m0216922/CG/floodfill.html#Recursive_Scanline_Floodfill_Algorithm (Copyright (c) 2004-2007, Lode Vandevenne)

bool ImageMatching::pop(int &x, int &y, int &sx, int &sy)
{
	if(stack != NULL)
	{
		int p = stack->coord;
		x = (p % img_width) + 1;
		y = (p / img_width) + 1;
		p = stack->scoord;
		sx = (p % simg_width) + 1;
		sy = (p / simg_width) + 1;

		aux = stack;
		stack = stack->prev;
		delete aux;

		return 1;
	}    
	else
	{
		return 0;
	}
}   
 
bool ImageMatching::push(int x, int y, int sx, int sy)
{
	aux = stack;
	stack = new stackCell();
	stack->coord = img_width * (y-1) + (x-1);
	stack->scoord = simg_width * (sy-1) + (sx-1);
	stack->prev = aux;
	return 1;
}    

void ImageMatching::emptyStack()
{
    int x, y, sx, sy;
    while(pop(x,y,sx,sy));
}

void ImageMatching::region_growing(Matrix *img1, Matrix *img2, MatchingPointsList *mpoints, int x, int y, int sx, int sy)
{
	int i,j,lsm_flag;
	int lx, ly, rx, ry;
        double new_x, new_y, p;
	
	emptyStack();
	push(x,y,sx,sy);

	while(pop(lx,ly,rx,ry))
	{
		i = (ly/step_y) + 1;
		j = (lx/step_x) + 1;

		// Check if current point is inside determined area
		if (lx < matching_xi || lx > matching_xf || ly < matching_yi || ly > matching_yf)
			continue;
		if (rx < smatching_xi || rx > smatching_xf || ry < smatching_yi || ry > smatching_yf)
			continue;

		// Check if visited
		if (map.get(i,j) != 0)
			continue;

		// Choose correlation method
		if (matching_method == NCC)
		{
			ncc.setTemplateCenter(lx, ly);
			ncc.setSearchWindowCenter(rx, ry);
			ncc.searchHomologous(img1,img2);
			p = ncc.getBestP();
			if (p < corr_th)
				continue;
			new_x = ncc.getBestX();
			new_y = ncc.getBestY();
	
			// Add 4-neighbor pixels - NCC - Faster if inside this 'if'
			// If new seed are based on new_x and new_y, may diverge !!
			push(lx + step_x, ly, rx + step_x, ry);
			push(lx - step_x, ly, rx - step_x, ry);
			push(lx, ly + step_y, rx, ry + step_y);
			push(lx, ly - step_y, rx, ry - step_y);
		}

		if (matching_method == LSM)
		{
			lsm_flag = lsm.searchHomologous(img1, img2, double(lx), double(ly), double(rx), double(ry));
			p = lsm.getBestP();
                        if (p < corr_th || lsm_flag < 1)
				continue;

			new_x = lsm.getBestX();
			new_y = lsm.getBestY();

			// Add 4-neighbor pixels - LSM - Faster if inside this 'if'
			push(lx + step_x, ly, new_x + step_x, new_y);
			push(lx - step_x, ly, new_x - step_x, new_y);
			push(lx, ly + step_y, new_x, new_y + step_y);
			push(lx, ly - step_y, new_x, new_y - step_y);
		}

		// Add pair to list, converting to c++ coordinates system
		mpoints->add(left_image_id, right_image_id, double(lx-1), double(ly-1), new_x-1.0, new_y-1.0, p);

		// Set visited
		map.set(i,j,1.0);

		// Calculate coverage
		num_visited += 1.0;
		coverage = num_visited/max_size;
                if (manager == NULL)
                    printf("%.2f %%\r",coverage*100);
                else
                    manager->setProgress(int(coverage*100));

                if (cancel_flag)
                    return;
	}
}

