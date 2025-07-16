/**************************************************************
***************************************************************
*** 
***   plot.c
***
***   Jason Olejarz
***   2011-01-27
***
***   This file contains all graphics functions for ising3d.
***
***************************************************************
**************************************************************/

#include "includes.h"

void animate_real_time(short int *spins, int *flips, double *energy, double *time, int steps_begin, int steps, double time_between_frames, int nx, int ny, short int spin, double delay, double time_offset, short int save_frames, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int i;
	int x, y;
	
	fprintf(matlab_data, "%d\n%d\n%d\n%d\n%d\n%d\n%lf\n", nx, ny, steps_begin, steps, (int)spin, (int)save_frames, time_offset);
	
	for (i=0; i<nx*ny; i++)
	{
		fprintf(matlab_data, "%d\n", *(spins+i));
	}
	
	fprintf(matlab_data, "%d\n", (int)(*energy));
	
	for (i=0; i<steps; i++)
	{
		y=(int)(*(flips+2*i)/nx);
		x=(int)(*(flips+2*i)-y*nx);

		fprintf(matlab_data, "%d %d\n", x, y);
		fprintf(matlab_data, "%d\n", *(flips+2*i+1));
		fprintf(matlab_data, "%d\n", (int)(*(energy+i+1)));
		fprintf(matlab_data, "%f\n", (float)(*(time+i+1)));
	}
	
	fprintf(matlab_plot, "hold on\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps_begin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "spin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "save_frames=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "time_offset=fscanf(plotdata, '%%f', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "s=zeros(nx,ny);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for y=1:ny\n");
	fprintf(matlab_plot, "	for x=1:nx\n");
	fprintf(matlab_plot, "		s(x,y)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "energy=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "last_time=0;\n");
//	fprintf(matlab_plot, "title(['energy/spin = ', num2str(energy/nx/ny), ',  time = ', num2str(time_offset)], 'FontSize', 20)\n");
	fprintf(matlab_plot, "title(['time = ', num2str(time_offset)], 'FontSize', 20)\n");
	fprintf(matlab_plot, "drawnow\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "imagecount=0;\n");
	fprintf(matlab_plot, "if save_frames==1\n");
	fprintf(matlab_plot, "  print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for i=1:steps\n");
	fprintf(matlab_plot, "	x=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	y=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	flip=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	energy=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	time=fscanf(plotdata, '%%f', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	s(x+1,y+1)=-s(x+1,y+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	handle_old=h(x+1, y+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if s(x+1,y+1)==spin\n");
	fprintf(matlab_plot, "		h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'w', 'EdgeColor', 'w');\n");
	fprintf(matlab_plot, "	else\n");
	fprintf(matlab_plot, "		h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'k', 'EdgeColor', 'k');\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	delete(handle_old);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "	if time-last_time>=%lf || i==steps\n",time_between_frames);
//	fprintf(matlab_plot, "		title(['energy/spin = ', num2str(energy/nx/ny), ',  time = ', num2str(time_offset+time)], 'FontSize', 20)\n");
	fprintf(matlab_plot, "		title(['time = ', num2str(time_offset+time)], 'FontSize', 20)\n");
	fprintf(matlab_plot, "		pause(%lf)\n", delay);
	fprintf(matlab_plot, "		drawnow\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		last_time=time;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "          imagecount=imagecount+1;\n");
	fprintf(matlab_plot, "          if save_frames==1\n");
	fprintf(matlab_plot, "                  print('-djpeg', strcat(num2str(imagecount, '%%03d'), '.jpg'));\n");
	fprintf(matlab_plot, "          end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "fclose(plotdata);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hold off\n");

	return;
}

void animate_real_time_step(short int *spins, int *flips, double *energy, double *time, int steps_begin, int steps, int nx, int ny, int nz, short int spin, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
/*
	int i;
	int x, y, z;
	
	fprintf(matlab_data, "%d\n%d\n%d\n%d\n%d\n%hd\n", nx, ny, nz, steps_begin, steps, spin);
	
	for (i=0; i<nx*ny*nz; i++)
	{
		fprintf(matlab_data, "%d\n", *(spins+i));
	}
	
	fprintf(matlab_data, "%d\n", (int)(*energy));
	
	for (i=0; i<steps; i++)
	{
		z=(int)(*(flips+2*i)/nx/ny);
		y=(int)((*(flips+2*i)-z*nx*ny)/nx);
		x=(int)(*(flips+2*i)-z*nx*ny-y*nx);

		fprintf(matlab_data, "%d %d %d\n", x, y, z);
		fprintf(matlab_data, "%d\n", *(flips+2*i+1));
		fprintf(matlab_data, "%d\n", (int)(*(energy+i+1)));
	}

	fprintf(matlab_plot, "function evolve_animate_step\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "global nx ny nz steps_begin steps spin old_step s hx hy hz x_array y_array z_array flip_array energy_array\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "nz=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps_begin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "spin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hx=zeros(nx+1, ny, nz);\n");
	fprintf(matlab_plot, "hy=zeros(nx, ny+1, nz);\n");
	fprintf(matlab_plot, "hz=zeros(nx, ny, nz+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "s=zeros(nx,ny,nz);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis([0 nx 0 ny 0 nz], 'equal')\n");
	fprintf(matlab_plot, "set(gca,'Units','centimeters')\n");
	fprintf(matlab_plot, "position=get(gcf,'Position');\n");
	fprintf(matlab_plot, "position(4)=500;\n");
	fprintf(matlab_plot, "set(gcf,'Position',position)\n");
	fprintf(matlab_plot, "xlabel('x')\n");
	fprintf(matlab_plot, "ylabel('y')\n");
	fprintf(matlab_plot, "zlabel('z')\n");
	fprintf(matlab_plot, "title(['lattice=(', num2str(nx), ', ', num2str(ny), ', ', num2str(nz), '), steps (min, current, max)=(', num2str(steps_begin), ', ', num2str(steps_begin), ', ', num2str(steps_begin+steps), ')'])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "slider=uicontrol('Style','slider','Callback',@slider_callback,'Min',steps_begin,'Max',steps_begin+steps,'SliderStep',[1/steps 1],'Value',steps_begin,'Position',[64 400 384 20]);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for z=0:nz-1\n");
	fprintf(matlab_plot, "	for y=0:ny-1\n");
	fprintf(matlab_plot, "		for x=0:nx-1\n");
	fprintf(matlab_plot, "			s(x+1,y+1,z+1)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "			if s(x+1,y+1,z+1)==spin\n");
	fprintf(matlab_plot, "				if hx(x+1, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hx(x+1, y+1, z+1)=patch([x x x x], [y y+1 y+1 y], [z z z+1 z+1], 'r');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hx(x+2, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hx(x+2, y+1, z+1)=patch([x+1 x+1 x+1 x+1], [y y+1 y+1 y], [z z z+1 z+1], 'r');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hy(x+1, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hy(x+1, y+1, z+1)=patch([x x x+1 x+1], [y y y y], [z z+1 z+1 z], 'g');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hy(x+1, y+2, z+1)==0\n");
	fprintf(matlab_plot, "					hy(x+1, y+2, z+1)=patch([x x x+1 x+1], [y+1 y+1 y+1 y+1], [z z+1 z+1 z], 'g');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hz(x+1, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hz(x+1, y+1, z+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'b');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hz(x+1, y+1, z+2)==0\n");
	fprintf(matlab_plot, "					hz(x+1, y+1, z+2)=patch([x x+1 x+1 x], [y y y+1 y+1], [z+1 z+1 z+1 z+1], 'b');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "x_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "y_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "z_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "flip_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "energy_array=zeros(steps,1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "old_step=steps_begin;\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for i=1:steps\n");
	fprintf(matlab_plot, "	x_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	y_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	z_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	flip_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	energy_array(i)=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "function slider_callback(h, eventdata)\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "global nx ny nz steps_begin steps spin old_step s hx hy hz x_array y_array z_array flip_array energy_array\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "set(h,'Value',cast(get(h,'Value'),'int32'))\n");
	fprintf(matlab_plot, "title(['lattice=(', num2str(nx), ', ', num2str(ny), ', ', num2str(nz), '), steps (min, current, max)=(', num2str(steps_begin), ', ', num2str(get(h,'Value')), ', ', num2str(steps_begin+steps), ')'])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "if get(h,'Value')~=old_step\n");
	fprintf(matlab_plot, "	if get(h,'Value')<old_step\n");
	fprintf(matlab_plot, "		loop_start=get(h,'Value')+1;\n");
	fprintf(matlab_plot, "		loop_end=old_step;\n");
	fprintf(matlab_plot, "	elseif get(h,'Value')>old_step\n");
	fprintf(matlab_plot, "		loop_start=old_step+1;\n");
	fprintf(matlab_plot, "		loop_end=get(h,'Value');\n");	
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "	for i=loop_start:loop_end\n");
	fprintf(matlab_plot, "		x=x_array(i-steps_begin);\n");
	fprintf(matlab_plot, "		y=y_array(i-steps_begin);\n");
	fprintf(matlab_plot, "		z=z_array(i-steps_begin);\n");
	fprintf(matlab_plot, "		flip=flip_array(i-steps_begin);\n");
	fprintf(matlab_plot, "		energy=energy_array(i-steps_begin);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		s(x+1,y+1,z+1)=-s(x+1,y+1,z+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "		if s(x+1,y+1,z+1)==spin\n");
	fprintf(matlab_plot, "			if (x>0 && s(x,y+1,z+1)==-spin) || x==0\n");
	fprintf(matlab_plot, "				hx(x+1, y+1, z+1)=patch([x x x x], [y y+1 y+1 y], [z z z+1 z+1], 'r');\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if (x<nx-1 && s(x+2,y+1,z+1)==-spin) || x==nx-1\n");
	fprintf(matlab_plot, "				hx(x+2, y+1, z+1)=patch([x+1 x+1 x+1 x+1], [y y+1 y+1 y], [z z z+1 z+1], 'r');\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if (y>0 && s(x+1,y,z+1)==-spin) || y==0\n");
	fprintf(matlab_plot, "				hy(x+1, y+1, z+1)=patch([x x x+1 x+1], [y y y y], [z z+1 z+1 z], 'g');\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if (y<ny-1 && s(x+1,y+2,z+1)==-spin) || y==ny-1\n");
	fprintf(matlab_plot, "				hy(x+1, y+2, z+1)=patch([x x x+1 x+1], [y+1 y+1 y+1 y+1], [z z+1 z+1 z], 'g');\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if (z>0 && s(x+1,y+1,z)==-spin) || z==0\n");
	fprintf(matlab_plot, "				hz(x+1, y+1, z+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'b');\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if (z<nz-1 && s(x+1,y+1,z+2)==-spin) || z==nz-1\n");
	fprintf(matlab_plot, "				hz(x+1, y+1, z+2)=patch([x x+1 x+1 x], [y y y+1 y+1], [z+1 z+1 z+1 z+1], 'b');\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			if (x>0 && s(x,y+1,z+1)==-spin) || x==0\n");
	fprintf(matlab_plot, "				delete(hx(x+1, y+1, z+1));\n");
	fprintf(matlab_plot, "				hx(x+1, y+1, z+1)=0;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if (x<nx-1 && s(x+2,y+1,z+1)==-spin) || x==nx-1\n");
	fprintf(matlab_plot, "				delete(hx(x+2, y+1, z+1));\n");
	fprintf(matlab_plot, "				hx(x+2, y+1, z+1)=0;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if (y>0 && s(x+1,y,z+1)==-spin) || y==0\n");
	fprintf(matlab_plot, "				delete(hy(x+1, y+1, z+1));\n");
	fprintf(matlab_plot, "				hy(x+1, y+1, z+1)=0;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if (y<ny-1 && s(x+1,y+2,z+1)==-spin) || y==ny-1\n");
	fprintf(matlab_plot, "				delete(hy(x+1, y+2, z+1));\n");
	fprintf(matlab_plot, "				hy(x+1, y+2, z+1)=0;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if (z>0 && s(x+1,y+1,z)==-spin) || z==0\n");
	fprintf(matlab_plot, "				delete(hz(x+1, y+1, z+1));\n");
	fprintf(matlab_plot, "				hz(x+1, y+1, z+1)=0;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "			if (z<nz-1 && s(x+1,y+1,z+2)==-spin) || z==nz-1\n");
	fprintf(matlab_plot, "				delete(hz(x+1, y+1, z+2));\n");
	fprintf(matlab_plot, "				hz(x+1, y+1, z+2)=0;\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "old_step=get(h,'Value');\n");
*/	
	return;
}

void cluster_spins_plot(short int *cluster_spins, int nx, int ny, int nz, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
/*
	int x, y, z;
	int nxtimesny=nx*ny;
	
	fprintf(matlab_data, "%d\n%d\n%d\n", nx, ny, nz);
	
	for (z=0; z<nz; z++)
	{
		for (y=0; y<ny; y++)
		{
			for (x=0; x<nx; x++)
			{
				fprintf(matlab_data, "%d\n", *(cluster_spins+x+nx*y+nxtimesny*z));
			}
		}
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "nz=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "hx=zeros(nx+1, ny, nz);\n");
	fprintf(matlab_plot, "hy=zeros(nx, ny+1, nz);\n");
	fprintf(matlab_plot, "hz=zeros(nx, ny, nz+1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis('off', 'equal', [0 nx 0 ny 0 nz])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for z=0:nz-1\n");
	fprintf(matlab_plot, "	for y=0:ny-1\n");
	fprintf(matlab_plot, "		for x=0:nx-1\n");
	fprintf(matlab_plot, "			if fscanf(plotdata, '%%d', 1)==1\n");
	fprintf(matlab_plot, "				if hx(x+1, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hx(x+1, y+1, z+1)=patch([x x x x], [y y+1 y+1 y], [z z z+1 z+1], 'r');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hx(x+2, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hx(x+2, y+1, z+1)=patch([x+1 x+1 x+1 x+1], [y y+1 y+1 y], [z z z+1 z+1], 'r');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hy(x+1, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hy(x+1, y+1, z+1)=patch([x x x+1 x+1], [y y y y], [z z+1 z+1 z], 'g');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hy(x+1, y+2, z+1)==0\n");
	fprintf(matlab_plot, "					hy(x+1, y+2, z+1)=patch([x x x+1 x+1], [y+1 y+1 y+1 y+1], [z z+1 z+1 z], 'g');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hz(x+1, y+1, z+1)==0\n");
	fprintf(matlab_plot, "					hz(x+1, y+1, z+1)=patch([x x+1 x+1 x], [y y y+1 y+1], [z z z z], 'b');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "				if hz(x+1, y+1, z+2)==0\n");
	fprintf(matlab_plot, "					hz(x+1, y+1, z+2)=patch([x x+1 x+1 x], [y y y+1 y+1], [z+1 z+1 z+1 z+1], 'b');\n");
	fprintf(matlab_plot, "				end\n");
	fprintf(matlab_plot, "			end\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
*/	
	return;
}

void plot_frame_movie(short int *spins, int nx, int ny, double time, int image_number, short int spin, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int x, y;
	int nxtimesny=nx*ny;
	
	fprintf(matlab_data, "%d\n%d\n%f\n%d\n", nx, ny, (float)time, image_number);
	
	for (y=0; y<ny; y++)
	{
		for (x=0; x<nx; x++)
		{
			fprintf(matlab_data, "%d\n", *(spins+x+nx*y));
		}
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "time=fscanf(plotdata, '%%f', 1);\n");
	fprintf(matlab_plot, "image_number=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "h=zeros(nx, ny);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis('off', 'equal', [0 nx -1-0.2 ny+1+0.2])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "title(['time = ', num2str(time,'%%.0f')], 'FontSize', 20)\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for y=0:ny-1\n");
	fprintf(matlab_plot, "	for x=0:nx-1\n");
	fprintf(matlab_plot, "		spin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		if spin==%d\n",spin);
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'b', 'EdgeColor', 'b');\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'y', 'EdgeColor', 'y');\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "print('-djpeg', strcat(num2str(image_number, '%%04d'), '.jpg'));\n");

	return;
}

void state_plot_matlab(short int *spins, int nx, int ny, int steps, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int x, y;
	int nxtimesny=nx*ny;
	
	fprintf(matlab_data, "%d\n%d\n%d\n", nx, ny, steps);
	
	for (y=0; y<ny; y++)
	{
		for (x=0; x<nx; x++)
		{
			fprintf(matlab_data, "%d\n", *(spins+x+nx*y));
		}
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "h=zeros(nx, ny);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis('off', 'equal', [0 nx -1-0.2 ny+1+0.2])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for y=0:ny-1\n");
	fprintf(matlab_plot, "	for x=0:nx-1\n");
	fprintf(matlab_plot, "		spin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		if spin==-1\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'w', 'EdgeColor', 'w');\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'k', 'EdgeColor', 'k');\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");

	return;
}

void state_plot_matlab_fixed_boundaries(short int *spins, short int *lower_boundary, short int *upper_boundary, int nx, int ny, int steps, char matlab_data_filename[], FILE *matlab_data, FILE *matlab_plot)
{
	int x, y;
	int nxtimesny=nx*ny;
	
	fprintf(matlab_data, "%d\n%d\n%d\n", nx, ny, steps);
	
	for (x=0; x<nx; x++)
	{
		fprintf(matlab_data, "%d\n", *(lower_boundary+x));
	}

	for (x=0; x<nx; x++)
	{
		fprintf(matlab_data, "%d\n", *(upper_boundary+x));
	}

	for (y=0; y<ny; y++)
	{
		for (x=0; x<nx; x++)
		{
			fprintf(matlab_data, "%d\n", *(spins+x+nx*y));
		}
	}
	
	fprintf(matlab_plot, "plotdata=fopen('%s', 'r');\n", matlab_data_filename);
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "nx=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "ny=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "steps=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "h=zeros(nx, ny);\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "figure\n");
	fprintf(matlab_plot, "axis('off', 'equal', [0 nx -1-0.2 ny+1+0.2])\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for x=0:nx-1\n");
	fprintf(matlab_plot, "	spin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	if spin==-1\n");
	fprintf(matlab_plot, "		patch([x x+1 x+1 x], [-1-0.3 -1-0.3 -0.3 -0.3], 'w', 'EdgeColor', 'w', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "	else\n");
	fprintf(matlab_plot, "		patch([x x+1 x+1 x], [-1-0.3 -1-0.3 -0.3 -0.3], 'k', 'EdgeColor', 'k', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for x=0:nx-1\n");
	fprintf(matlab_plot, "	spin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "	if spin==-1\n");
	fprintf(matlab_plot, "		patch([x x+1 x+1 x], [ny+0.3 ny+0.3 ny+1+0.3 ny+1+0.3], 'w', 'EdgeColor', 'w', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "	else\n");
	fprintf(matlab_plot, "		patch([x x+1 x+1 x], [ny+0.3 ny+0.3 ny+1+0.3 ny+1+0.3], 'k', 'EdgeColor', 'k', 'LineWidth', 2);\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");
	fprintf(matlab_plot, "\n");
	fprintf(matlab_plot, "for y=0:ny-1\n");
	fprintf(matlab_plot, "	for x=0:nx-1\n");
	fprintf(matlab_plot, "		spin=fscanf(plotdata, '%%d', 1);\n");
	fprintf(matlab_plot, "		if spin==-1\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'w', 'EdgeColor', 'w');\n");
	fprintf(matlab_plot, "		else\n");
	fprintf(matlab_plot, "			h(x+1, y+1)=patch([x x+1 x+1 x], [y y y+1 y+1], 'k', 'EdgeColor', 'k');\n");
	fprintf(matlab_plot, "		end\n");
	fprintf(matlab_plot, "	end\n");
	fprintf(matlab_plot, "end\n");

	return;
}
