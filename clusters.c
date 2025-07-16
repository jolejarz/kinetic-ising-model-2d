/**************************************************************
***************************************************************
*** 
***   clusters.c
***
***   Jason Olejarz
***   2010-07-21
***
***   This file contains routines for determining cluster statistics.
***
***************************************************************
**************************************************************/

#include "includes.h"

void clusters (struct run_parameters *parameters, int *lattice, short int *spins)
{
	int nx=parameters->xlen, ny=parameters->ylen;
	int nxtimesny=nx*ny;
	
	int x, y;
	
	int xminus, yminus;
	
	int current_spin, test_spin;
	
	int test_spin_1, test_spin_2;
	
	int i, j, k, n;
	
	int *find_spin, *find_cluster;
	
	int cluster_current, cluster_test;
	
	// Initialize 'lattice'.
	for (i=0; i<nxtimesny; i++)
	{
		*(lattice+i)=i;
	}

	for (y=0; y<ny; y++)
	{
		for (x=0; x<nx; x++)
		{
			// Determinine the coordinates of the neighboring spins.
			// If the boundaries are free along a direction and the flipped spin is at a boundary
			// where there is no neighboring spin, the coordinates of the missing neighbor are set
			// to the flipped spin's coordinates. This signals that there are free boundaries,
			// and special processing is done.
			if (x>0) xminus=x-1; else xminus=parameters->periodic_bc_x==1 ? nx-1 : x;
			if (y>0) yminus=y-1; else yminus=parameters->periodic_bc_y==1 ? ny-1 : y;

			current_spin=x+nx*y;

			// Compare the selected spin with its three neighboring spins at
			// (xminus, y, z), (x, yminus, z), and (x, y, zminus).
			//
			// If two spins are equal and they belong to the same cluster,
			// there is nothing to be done.
			//
			// If two spins are equal and neither already belongs to a cluster,
			// make a new cluster for both of them.
			//
			// If two spins are equal and only the test spin belongs to a cluster,
			// add the current spin to the test spin's cluster.
			//
			// If two spins are equal and only the current spin belongs to a cluster,
			// add the test spin to the current spin's cluster.
			//
			// If two spins are equal and they belong to different clusters,
			// make all spins in the test spin's cluster point to the current spin's
			// cluster.
			//
			for (i=0; i<2; i++)
			{
				switch (i)
				{
					case 0: {test_spin=xminus+nx*y; break;}
					case 1: {test_spin=x+nx*yminus; break;}
				}
				
				// If there is no test spin due to free boundaries, do not do any processing.
				if (current_spin==test_spin) continue;
				
				if (*(spins+current_spin)==*(spins+test_spin))
				{
					// The current spin and test spin are equal.

					// Find the clusters of the two spins.
					for (n=0; n<2; n++)
					{
						switch (n)
						{
							case 0: {find_spin=&current_spin; find_cluster=&cluster_current; break;}
							case 1: {find_spin=&test_spin; find_cluster=&cluster_test; break;}
						}
						
						// Find the cluster of the spin.
						*find_cluster=*find_spin;
						while (*find_cluster!=*(lattice+*find_cluster)) *find_cluster=*(lattice+*find_cluster);

						// Compress the path from test_spin to j.
						j=*find_spin;
						while (j!=*(lattice+j))
						{
							k=*(lattice+j);
							*(lattice+j)=*find_cluster;
							j=k;
						}
					}

					// Make the current cluster point to the test cluster.
					*(lattice+cluster_current)=cluster_test;
				}
			}
		}
	}
	
	// All clusters have been labeled.

	return;
}

void clusters_percolation (struct run_parameters *parameters, int *lattice, short int *spins, int *displacement_x, int *displacement_y, int *winding_a_lattice, int *winding_b_lattice, int *stripes_count, int *winding_a_max, int *winding_b_max)
{
	int nx=parameters->xlen, ny=parameters->ylen;
	int nxtimesny=nx*ny;
	
	int x, y;
	
	int xminus, yminus;
	
	int current_spin, test_spin;
	
	int test_spin_1, test_spin_2;
	
	int i, j, k, n;
	
	int *find_spin, *find_cluster;
	
	int cluster_current, cluster_test;

	int total_displacement_x, total_displacement_y;
	int total_displacement_x_old, total_displacement_y_old;
	
	int winding_x, winding_y;

	int final_cluster;

	// Initialize the arrays.
	for (i=0; i<nxtimesny; i++)
	{
		*(lattice+i)=i;
		*(displacement_x+i)=0;
		*(displacement_y+i)=0;
		*(winding_a_lattice+i)=0;
		*(winding_b_lattice+i)=0;
		*stripes_count=0;
	}

	for (y=0; y<ny; y++)
	{
		for (x=0; x<nx; x++)
		{
			// Determinine the coordinates of the neighboring spins.
			// If the boundaries are free along a direction and the flipped spin is at a boundary
			// where there is no neighboring spin, the coordinates of the missing neighbor are set
			// to the flipped spin's coordinates. This signals that there are free boundaries,
			// and special processing is done.
			if (x>0) xminus=x-1; else xminus=parameters->periodic_bc_x==1 ? nx-1 : x;
			if (y>0) yminus=y-1; else yminus=parameters->periodic_bc_y==1 ? ny-1 : y;

			current_spin=x+nx*y;

			// Compare the selected spin with its three neighboring spins at
			// (xminus, y, z), (x, yminus, z), and (x, y, zminus).
			//
			// If two spins are equal and they belong to the same cluster,
			// there is nothing to be done.
			//
			// If two spins are equal and neither already belongs to a cluster,
			// make a new cluster for both of them.
			//
			// If two spins are equal and only the test spin belongs to a cluster,
			// add the current spin to the test spin's cluster.
			//
			// If two spins are equal and only the current spin belongs to a cluster,
			// add the test spin to the current spin's cluster.
			//
			// If two spins are equal and they belong to different clusters,
			// make all spins in the test spin's cluster point to the current spin's
			// cluster.
			//
			for (i=0; i<2; i++)
			{
				switch (i)
				{
					case 0: {test_spin=xminus+nx*y; break;}
					case 1: {test_spin=x+nx*yminus; break;}
				}
				
				// If there is no test spin due to free boundaries, do not do any processing.
				if (current_spin==test_spin) continue;
				
				if (*(spins+current_spin)==*(spins+test_spin))
				{
					// The current spin and test spin are equal.

					// Find the clusters of the two spins.
					for (n=0; n<2; n++)
					{
						switch (n)
						{
							case 0: {find_spin=&current_spin; find_cluster=&cluster_current; break;}
							case 1: {find_spin=&test_spin; find_cluster=&cluster_test; break;}
						}
						
						// Find the cluster of the spin.
						*find_cluster=*find_spin;
						total_displacement_x=0;
						total_displacement_y=0;
						while (*find_cluster!=*(lattice+*find_cluster))
						{
							total_displacement_x+=*(displacement_x+(*find_cluster));
							total_displacement_y+=*(displacement_y+(*find_cluster));
							*find_cluster=*(lattice+*find_cluster);
						}

						// Compress the path from test_spin to j.
						j=*find_spin;
						while (j!=*(lattice+j))
						{
							k=*(lattice+j);

							total_displacement_x_old=*(displacement_x+j);
							total_displacement_y_old=*(displacement_y+j);

							*(lattice+j)=*find_cluster;

							*(displacement_x+j)=total_displacement_x;
							*(displacement_y+j)=total_displacement_y;

							total_displacement_x-=total_displacement_x_old;
							total_displacement_y-=total_displacement_y_old;

							j=k;
						}
						*(displacement_x+j)=0;
						*(displacement_y+j)=0;
					}

					// Make the current cluster point to the test cluster.
					*(lattice+cluster_current)=cluster_test;

					// Set the distance from the root node of the current cluster
					// to the root node of the test cluster.
					if (i==0)
					{
						*(displacement_x+cluster_current)=1+*(displacement_x+test_spin)-*(displacement_x+current_spin);
						*(displacement_y+cluster_current)=*(displacement_y+test_spin)-*(displacement_y+current_spin);
					}
					else
					{
						*(displacement_x+cluster_current)=*(displacement_x+test_spin)-*(displacement_x+current_spin);
						*(displacement_y+cluster_current)=1+*(displacement_y+test_spin)-*(displacement_y+current_spin);
					}
				}
			}
		}
	}

	// Compress all paths.
	for (i=0; i<nxtimesny; i++)
	{
		// Find the cluster of the spin.
		j=i;
		total_displacement_x=0;
		total_displacement_y=0;
		while (j!=*(lattice+j))
		{
			k=*(lattice+j);
			total_displacement_x+=*(displacement_x+j);
			total_displacement_y+=*(displacement_y+j);
			j=k;
		}
		final_cluster=j;

		// Compress the path.
		j=i;
		while (j!=*(lattice+j))
		{
			k=*(lattice+j);

			total_displacement_x_old=*(displacement_x+j);
			total_displacement_y_old=*(displacement_y+j);

			*(lattice+j)=final_cluster;

			*(displacement_x+j)=total_displacement_x;
			*(displacement_y+j)=total_displacement_y;

			total_displacement_x-=total_displacement_x_old;
			total_displacement_y-=total_displacement_y_old;

			j=k;
		}
		*(displacement_x+j)=0;
		*(displacement_y+j)=0;
	}

	// Get statistics on all stripes.
	*stripes_count=0;
	*winding_a_max=0;
	*winding_b_max=0;
	for (y=0; y<ny; y++)
	{
		for (x=0; x<nx; x++)
		{
			// Determinine the coordinates of the neighboring spins.
			// If the boundaries are free along a direction and the flipped spin is at a boundary
			// where there is no neighboring spin, the coordinates of the missing neighbor are set
			// to the flipped spin's coordinates. This signals that there are free boundaries,
			// and special processing is done.
			if (x>0) xminus=x-1; else xminus=parameters->periodic_bc_x==1 ? nx-1 : x;
			if (y>0) yminus=y-1; else yminus=parameters->periodic_bc_y==1 ? ny-1 : y;

			current_spin=x+nx*y;

			for (i=0; i<2; i++)
			{
				switch (i)
				{
					case 0: {test_spin=xminus+nx*y; break;}
					case 1: {test_spin=x+nx*yminus; break;}
				}
				
				// If there is no test spin due to free boundaries, do not do any processing.
				if (current_spin==test_spin) continue;
				
				// Check if the two spins belong to the same cluster.
				if (*(lattice+current_spin)==*(lattice+test_spin))
				{
					// Check if there is percolation.
					if (i==0)
					{
						if (abs((*(displacement_x+test_spin))-(*(displacement_x+current_spin)))>1)
						{
							// If this is the first time this cluster has percolated, then increment `stripes_count'.
							if (*(winding_a_lattice+(*(lattice+current_spin)))==0 && *(winding_b_lattice+(*(lattice+current_spin)))==0) (*(stripes_count))++;

							if (*(winding_a_lattice+(*(lattice+current_spin)))<(abs((*(displacement_x+test_spin))-(*(displacement_x+current_spin)))+1)/nx) *(winding_a_lattice+(*(lattice+current_spin)))=(abs((*(displacement_x+test_spin))-(*(displacement_x+current_spin)))+1)/nx;
							
							if (*(winding_a_lattice+(*(lattice+current_spin)))>*winding_a_max) *winding_a_max=*(winding_a_lattice+(*(lattice+current_spin)));
						}
						if (abs((*(displacement_y+test_spin))-(*(displacement_y+current_spin)))>1)
						{
							// If this is the first time this cluster has percolated, then increment `stripes_count'.
							if (*(winding_a_lattice+(*(lattice+current_spin)))==0 && *(winding_b_lattice+(*(lattice+current_spin)))==0) (*(stripes_count))++;

							if (*(winding_b_lattice+(*(lattice+current_spin)))<(abs((*(displacement_y+test_spin))-(*(displacement_y+current_spin))))/ny) *(winding_b_lattice+(*(lattice+current_spin)))=(abs((*(displacement_y+test_spin))-(*(displacement_y+current_spin))))/ny;
							
							if (*(winding_b_lattice+(*(lattice+current_spin)))>*winding_b_max) *winding_b_max=*(winding_b_lattice+(*(lattice+current_spin)));
						}
					}
					else
					{
						if (abs((*(displacement_y+test_spin))-(*(displacement_y+current_spin)))>1)
						{
							// If this is the first time this cluster has percolated, then increment `stripes_count'.
							if (*(winding_a_lattice+(*(lattice+current_spin)))==0 && *(winding_b_lattice+(*(lattice+current_spin)))==0) (*(stripes_count))++;

							if (*(winding_b_lattice+(*(lattice+current_spin)))<(abs((*(displacement_y+test_spin))-(*(displacement_y+current_spin)))+1)/ny) *(winding_b_lattice+(*(lattice+current_spin)))=(abs((*(displacement_y+test_spin))-(*(displacement_y+current_spin)))+1)/ny;
							
							if (*(winding_b_lattice+(*(lattice+current_spin)))>*winding_b_max) *winding_b_max=*(winding_b_lattice+(*(lattice+current_spin)));
						}
						if (abs((*(displacement_x+test_spin))-(*(displacement_x+current_spin)))>1)
						{
							// If this is the first time this cluster has percolated, then increment `stripes_count'.
							if (*(winding_a_lattice+(*(lattice+current_spin)))==0 && *(winding_b_lattice+(*(lattice+current_spin)))==0) (*(stripes_count))++;

							if (*(winding_a_lattice+(*(lattice+current_spin)))<(abs((*(displacement_x+test_spin))-(*(displacement_x+current_spin))))/nx) *(winding_a_lattice+(*(lattice+current_spin)))=(abs((*(displacement_x+test_spin))-(*(displacement_x+current_spin))))/nx;
							
							if (*(winding_a_lattice+(*(lattice+current_spin)))>*winding_a_max) *winding_a_max=*(winding_a_lattice+(*(lattice+current_spin)));
						}
					}
				}
			}
		}
	}
	*stripes_count/=2;
	
	// All clusters have been labeled.

	return;
}

// 'clusters' must be called before calling 'cluster_stats'.
void cluster_stats (struct run_parameters *parameters, struct cluster_stats *cluster_list, int *lattice, short int *spins)
{
	int i, j, k, number_of_clusters=0, size_of_cluster, nxtimesny=(parameters->xlen)*(parameters->ylen);
	
	// Initialize 'cluster_list' to all zeros.
	memset (cluster_list, 0, sizeof(struct cluster_stats)*nxtimesny);
	
	// Determine which spins point to themselves.
	// The locations of these spins are the cluster labels.
	for (i=0; i<nxtimesny; i++)
	{
		if (*(lattice+i)==i)
		{
			// Determine how many spins are in the cluster.
			size_of_cluster=0;
			for (j=0; j<nxtimesny; j++)
			{
				k=j;
				while (k!=*(lattice+k)) k=*(lattice+k);
				if (k==i) size_of_cluster++;
			}
			
			// Save the cluster's stats.
			(cluster_list+number_of_clusters)->label=i;
			(cluster_list+number_of_clusters)->number_of_spins=size_of_cluster;
			(cluster_list+number_of_clusters)->sign=*(spins+i);
			
			// Increment the number of clusters.
			number_of_clusters++;
		}
	}
	
	return;
}

// 'cluster_stats' must be called before calling 'save_cluster_spins'.
void save_cluster_spins (struct run_parameters *parameters, short int *cluster_spins, int cluster_number, struct cluster_stats *cluster_list, int *lattice)
{
	// All spins in cluster 'cluster_number' are written to 'cluster_spins'.

	int nxtimesny=(parameters->xlen)*(parameters->ylen);
	
	int i, j, k, size_of_cluster=0;
	
	// Zero all elements of 'cluster_spins'.
	memset (cluster_spins, 0, sizeof(short int)*nxtimesny);
	
	// Get the label for this cluster.
	int label=(cluster_list+cluster_number)->label;

	// Determine which spins are in the cluster.
	for (j=0; j<nxtimesny; j++)
	{
		k=j;
		while (k!=*(lattice+k)) k=*(lattice+k);
		
		// If spin 'j' is in the cluster, then set it to '1'.
		if (k==label) *(cluster_spins+j)=1;
	}

	return;
}
