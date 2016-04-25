#include "collector.hpp"

Collector::Collector()
{
}

int Collector::init()
{
  GDALAllRegister();
  const char *pszFormat = "GTiff";
  GDALDriver *poDriver;
  char **papszMetadata;
  poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
  if( poDriver == NULL )
    exit( 1 );
  papszMetadata = poDriver->GetMetadata();

  const char * pszSrcFilename = "SampleTiff.tif";
  GDALDataset *poSrcDS = 
    (GDALDataset *) GDALOpen( pszSrcFilename, GA_ReadOnly );

  GDALRasterBand *poBand;
  poBand = poSrcDS->GetRasterBand(1);

  Ndim = poBand->GetXSize();
  Mdim = poBand->GetYSize();
  
  GDALClose( (GDALDatasetH) poSrcDS );

  long int L = Ndim*Mdim;
  WeightMatrix1 = new double[L];
  WeightMatrix2 = new double[L];
  if(WeightMatrix1==NULL | WeightMatrix2==NULL)
    {
      std::cout<<"Allocating storage for WeightMatrix FAILED!"<< "\n";
      return -1;
    }
  return 0;
}

int Collector::print_trajctList()
{
  for (int i=0; i<trajctList.size(); i++)
    {
      for (int j=0; j<trajctList[i].grid.size(); j++)
	{
	  printf("%d[%d]", trajctList[i].grid[j], trajctList[i].direct[j]);
	}
      printf("\n");
    }
  return 0;
}

int Collector::make_weight_matrix()
{
  int i,j,k;
  for (k=0; k<trajctList.size(); k++)
    {
      // loop through trajctList
      // trajctList[k].grid is a vector
      // trajctList[k].direct is a vector
    }

  for (i=0; i<Ndim; i++)
    {
      for (j=0; j<Mdim; j++)
	{
	  WeightMatrix1[i*Ndim+j] = i*Ndim+j;
	  WeightMatrix2[i*Ndim+j] = 1.0/double(i*Ndim+j);
	}
    }
  return 0;
}

int Collector::output_weight_matrix()
{
  GDALAllRegister();
  const char *pszFormat = "GTiff";
  GDALDriver *poDriver;
  char **papszMetadata;
  poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
  if( poDriver == NULL )
    exit( 1 );
  papszMetadata = poDriver->GetMetadata();
  // if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATE, FALSE ) )
  //   printf( "Driver %s supports Create() method.\n", pszFormat );
  // if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATECOPY, FALSE ) )
  //   printf( "Driver %s supports CreateCopy() method.\n", pszFormat );

  const char * pszSrcFilename = "SampleTiff.tif";
  const char * pszDstFilename = "WM.tif";
  GDALDataset *poSrcDS = 
    (GDALDataset *) GDALOpen( pszSrcFilename, GA_ReadOnly );
  GDALDataset *poDstDS;
  poDstDS = poDriver->CreateCopy( pszDstFilename, poSrcDS, FALSE, 
				  NULL, NULL, NULL );
  // GDALDataset *poDstDS;       
  // const char * pszDstFilename = "WM.tif";
  // char **papszOptions = NULL;
  // poDstDS = poDriver->Create( pszDstFilename, Ndim, Mdim, 2, GDT_Byte, 
  // 			      papszOptions );
  // double adfGeoTransform[6] = { 444720, 30, 0, 3751320, 0, -30 };
  // OGRSpatialReference oSRS;
  // char *pszSRS_WKT = NULL;
  // //GByte abyRaster[512*512];
  // poDstDS->SetGeoTransform( adfGeoTransform );
  // oSRS.SetUTM( 11, TRUE );
  // oSRS.SetWellKnownGeogCS( "NAD27" );
  // oSRS.exportToWkt( &pszSRS_WKT );
  // poDstDS->SetProjection( pszSRS_WKT );
  // CPLFree( pszSRS_WKT );

  GDALRasterBand *poBand;
  poBand = poDstDS->GetRasterBand(1);
  poBand->RasterIO( GF_Write, 0, 0, Ndim, Mdim, 
		    WeightMatrix1, Ndim, Mdim, GDT_Float64, 0, 0 );    
  poBand = poDstDS->GetRasterBand(2);
  poBand->RasterIO( GF_Write, 0, 0, Ndim, Mdim, 
		    WeightMatrix2, Ndim, Mdim, GDT_Float64, 0, 0 );    
  // for (int i = 3; i<= poDstDS->GetRasterCount(); i++)
  //   {
  //     poDstDS->DeleteLayer()
  //   }
  /* Once we're done, close properly the dataset */
  //GDALClose( (GDALDatasetH) poDstDS );

  GDALClose( (GDALDatasetH) poDstDS );
  //GDALClose( (GDALDatasetH) poSrcDS );


  // int i,j;
  // for (i=0; i<Ndim; i++)
  //   {
  //     for (j=0; j<Mdim; j++)
  // 	{
  // 	  printf("%d %d %lf %lf \n", i, j, WeightMatrix1[i*Ndim+j], WeightMatrix2[i*Ndim+j]);
  // 	}
  //   }
  return 0;
}
