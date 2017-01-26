#include <iostream>
#include <stdio.h>

#include "eye_pts_detection.h"
#include "global.h"

int main ()
{
	//  cvSetImageROI(img, *eye_r);

	IplImage* img = cvLoadImage("../image_data/eye.jpeg", 1 );
	IplImage* gray_img =   cvCreateImage(cvSize(img->width, img->height), 8, 1);

	IplImage* thres_img =  cvCreateImage(cvSize(img->width, img->height), 8, 1);

	cvCvtColor(img, gray_img, CV_RGB2GRAY);

	// Threshold the eye image using simple/adaptive thresholding algorithm

	float threshold_value=0.0;
	  int k=5;

	  adaptive_threshold(gray_img, threshold_value,  thres_img,  k);


	  cvNamedWindow("src", 1);

	  cvShowImage("src", thres_img) ;

	    cvWaitKey(wait_ms);
	    cvSaveImage("threshold_img.jpeg", thres_img);

	  CvSeq* contour = NULL;  CvSeq* largest_contour = NULL;
	  CvMemStorage* storage = cvCreateMemStorage(0);
	  // CvRect object;

	  // int cts = cvFindContours(img, storage, &contour,
	  // 			   sizeof(CvContour),
	  // 			   CV_RETR_CCOMP,
	  // 			   CV_CHAIN_APPROX_SIMPLE);




	  int cts = cvFindContours(thres_img, storage, &contour);
	  cout<<"contour detected"<<"number of contours detected : "<<cts<<endl;
	  // cvClearMemStorage(storage);


	  if( contour )//find the largest contour
	    {
	      if(cts>1)
		{
	      double area, max=0;
	      for( ; contour != 0; contour = contour->h_next ){
	      	area = cvContourArea( contour , CV_WHOLE_SEQ );

	      	if (area > max )
	      	  {max = area;

	      	    largest_contour=contour;

	      	  }


	      }
		}

	      else
		largest_contour=contour;

	      contour_area = cvContourArea( largest_contour , CV_WHOLE_SEQ );

	      //draw the largest contour into the gray scale image
	      cvDrawContours(
	  		     out,
	  		     largest_contour,
	  		     cvScalarAll(255),
	  		     cvScalarAll(255),
	  		     -1,

	  		     CV_FILLED,
	  		     8,
	  		     cvPoint(0,0)
	  		     );




	      //clear the sequence


	      //obtain the eye corner points and eyelid_upper_mid and eyelid_lower_mid points
	      //*****************************************************************//


	      //**************************read the coordinate values of the sequence element for largest_contour**************************//
	      CvPoint  pt;
	      CvSeqReader reader;
	      CvPoint point_array[largest_contour->total];

	      cvStartReadSeq( largest_contour, &reader, 0);

	      for (int i=0; i<largest_contour->total;i++)
	  	{
	  	  CV_READ_SEQ_ELEM(pt, reader);
	  	  printf("x = %d y= %d\n", pt.x, pt.y);

	  	}
	      //********************************************************************//
	      //convert seq to array ********//
	      // CvPoint* point_array;
	      //fprintf(stderr,"malloc\n");
	      // Alloc memory for contour point set.



	      int contour_count=0;
	      cvCvtSeqToArray(largest_contour, point_array, CV_WHOLE_SEQ);

	      //   cout<<"seq array element are :"<<endl;

	      for(int j=0; j<largest_contour->total; j++)
		//	cout<<point_array[j].x<<"\t"<<point_array[j].y<<endl;

	      contour_count=largest_contour->total;


	      //call the function of eyebrow point location here:



	      //******find the bounding rectangle of the contour ************//
	      CvRect boundbox;
	      boundbox = cvBoundingRect(largest_contour);
	      CvPoint center;
	      center.x=(boundbox.x + boundbox.width)/2;
	      center.y=(boundbox.y + boundbox.height)/2;
	      float constraint_height=boundbox.height/5;
	      bool flag=false;
	      //************end of bounding rectangele***********************//
	      CvPoint eye_upper_mid, eye_lower_mid, eye_left, eye_right;

	      locate_pts( contour_count, point_array, center, constraint_height, eye_upper_mid,eye_lower_mid, eye_left,eye_right, flag);




	      draw_circle(img,eye_upper_mid);
	      draw_circle(img,eye_lower_mid);
	      draw_circle(img,eye_left);
	      draw_circle(img,eye_right);



	      // cvShowImage("src", pseudo_hue_img) ;
	      cvSaveImage("contour.jpeg", out);
	      cvSaveImage("final.jpeg", img);
	      //   cvWaitKey(0);




	      //  std::cout<<"contour area= \t "<<contour_area<<std::endl;
	      //  cout<<"contour area= \t" <<contour_area<<endl;
	      // cvClearSeq( contour);
	      // cvClearSeq( largest_contour);
	      //********************************************************//

	      //***********obtain the vertical and horizontal projection data **************//

	      max_ver_proj= vertical_projection(out);
	      max_hor_proj=horizontal_projection(out);


	      // max_ver_proj= vertical_projection(gray_img);// for testing
	      // max_hor_proj=horizontal_projection(gray_img);// for testing


	      // cout<<"max ver proj "<<max_ver_proj<<endl;
	      //cout<<"max hor proj "<<max_hor_proj<<endl;


	      proj_ratio=(max_ver_proj/max_hor_proj)*100;// 10 is a scaling factor

	      // char filename1[]="eye_projection_ratio.txt";
	      // write_scalar_data_to_file(filename1,  proj_ratio );

	      // cout<<" projection ratio= \t "<<proj_ratio<<endl;

	      //*********end of obtaining horizontal and vertical projection data *************//



	      cvResetImageROI(img);//reset the image to original size



	    }

	  else
	    {
	      std::cout<<"no contour :("<<std::endl;
	      return 0;
	    }

	  cvClearMemStorage(storage);
	  cvReleaseMemStorage(&storage);
	  // cvClearSeq( contour);
	  // cvClearSeq( largest_contour);

	  //********************************************************//



	  cvDestroyWindow("src");
	  cvReleaseImage(&gray_img);
	  cvReleaseImage(&thres_img);
	  cvReleaseImage(&out);
	  cvReleaseImage(&inp_img);
	  cvReleaseImage(&img);






	return 0;
}
//====End of main function ====//
//====================================================//
