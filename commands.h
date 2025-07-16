/**************************************************************
***************************************************************
*** 
***   commands.h
***
***   Jason Olejarz
***   2011-01-25
***
***   This file sets up the command arrays.
***
***************************************************************
**************************************************************/

enum {type_double, type_int, type_short_int, type_string, type_unsigned_long_int};

double arg_double[32];
int arg_int[32];
short int arg_short_int[32];
char arg_string[32][32];
unsigned long int arg_unsigned_long_int[32];

// data
void cmd_bins();
void cmd_cnt();
void cmd_dd();
void cmd_dindex();
void cmd_disp();
void cmd_dt();
void cmd_flt();
void cmd_fltintf();
void cmd_index();
void cmd_minmax();
void cmd_receive();
void cmd_sd();
void cmd_send();
void cmd_tag();
// file
void cmd_load();
void cmd_loadtext();
void cmd_save();
void cmd_savetext();
void cmd_xy();
// list
void cmd_dl();
void cmd_sl();
// loop
void cmd_lp();
void cmd_lpseed();
void cmd_randinit();
void cmd_randinitL();
void cmd_randinitU();
void cmd_tsc();
// plot
void cmd_anrt();
void cmd_anstep();
void cmd_clspins();
void cmd_spins();
void cmd_spinsfix();
// run
void cmd_rundiag();
void cmd_runfixper();
void cmd_runperper();
// save
void cmd_fbr();
void cmd_fcl();
void cmd_fe();
void cmd_finalper();
void cmd_fintf();
void cmd_fm();
void cmd_fmabs();
void cmd_fperc();
void cmd_fs();
void cmd_ft();
void cmd_incT();
void cmd_svseed();
// set
void cmd_af();
void cmd_blkinit();
void cmd_block();
void cmd_blockrev();
void cmd_cpcur();
void cmd_cpinit();
void cmd_diag1();
void cmd_diag2();
void cmd_reset();
void cmd_rnglcg();
void cmd_rngmt();
void cmd_seed();
void cmd_setdiag();
void cmd_setfixper();
void cmd_setperper();
void cmd_settsc();
// step
void cmd_energy();
void cmd_equstop();
void cmd_equtable();
void cmd_flips();
void cmd_mag();
void cmd_movie1();
void cmd_movie2();
void cmd_movie3();
void cmd_slabstop();
void cmd_stopdiag();
void cmd_swstop();
void cmd_time();
void cmd_trigs();
void cmd_trigt();
void cmd_trigtonce();

int main();

char *cmd_name[]=
{
	/* data */
	"bins",
	"cnt",
	"dd",
	"dindex",
	"disp",
	"dt",
	"flt",
	"fltintf",
	"index",
	"minmax",
	"receive",
	"sd",
	"send",
	"tag",
	/* file */
	"load",
	"loadtext",
	"save",
	"savetext",
	"xy",
	/* list */
	"dl",
	"sl",
	/* loop */
	"lp",
	"lpseed",
	"randinit",
	"randinitL",
	"randinitU",
	"tsc",
	/* plot */
	"anrt",
	"anstep",
	"clspins",
	"spins",
	"spinsfix",
	/* run */
	"rundiag",
	"runfixper",
	"runperper",
	/* save */
	"fbr",
	"fcl",
	"fe",
	"finalper",
	"fintf",
	"fm",
	"fmabs",
	"fperc",
	"fs",
	"ft",
	"incT",
	"svseed",
	/* set */
	"af",
	"blkinit",
	"block",
	"blockrev",
	"cpcur",
	"cpinit",
	"diag1",
	"diag2",
	"reset",
	"rnglcg",
	"rngmt",
	"seed",
	"setdiag",
	"setfixper",
	"setperper",
	"settsc",
	/* step */
	"energy",
	"equstop",
	"equtable",
	"flips",
	"mag",
	"movie1",
	"movie2",
	"movie3",
	"slabstop",
	"stopdiag",
	"swstop",
	"time",
	"trigs",
	"trigt",
	"trigtonce",
	/* exit / quit */
	"exit",
	"quit"
};
void (*functions[])()=
{
	/* data */
	cmd_bins,
	cmd_cnt,
	cmd_dd,
	cmd_dindex,
	cmd_disp,
	cmd_dt,
	cmd_flt,
	cmd_fltintf,
	cmd_index,
	cmd_minmax,
	cmd_receive,
	cmd_sd,
	cmd_send,
	cmd_tag,
	/* file */
	cmd_load,
	cmd_loadtext,
	cmd_save,
	cmd_savetext,
	cmd_xy,
	/* list */
	cmd_dl,
	cmd_sl,
	/* loop */
	cmd_lp,
	cmd_lpseed,
	cmd_randinit,
	cmd_randinitL,
	cmd_randinitU,
	cmd_tsc,
	/* plot */
	cmd_anrt,
	cmd_anstep,
	cmd_clspins,
	cmd_spins,
	cmd_spinsfix,
	/* run */
	cmd_rundiag,
	cmd_runfixper,
	cmd_runperper,
	/* save */
	cmd_fbr,
	cmd_fcl,
	cmd_fe,
	cmd_finalper,
	cmd_fintf,
	cmd_fm,
	cmd_fmabs,
	cmd_fperc,
	cmd_fs,
	cmd_ft,
	cmd_incT,
	cmd_svseed,
	/* set */
	cmd_af,
	cmd_blkinit,
	cmd_block,
	cmd_blockrev,
	cmd_cpcur,
	cmd_cpinit,
	cmd_diag1,
	cmd_diag2,
	cmd_reset,
	cmd_rnglcg,
	cmd_rngmt,
	cmd_seed,
	cmd_setdiag,
	cmd_setfixper,
	cmd_setperper,
	cmd_settsc,
	/* step */
	cmd_energy,
	cmd_equstop,
	cmd_equtable,
	cmd_flips,
	cmd_mag,
	cmd_movie1,
	cmd_movie2,
	cmd_movie3,
	cmd_slabstop,
	cmd_stopdiag,
	cmd_swstop,
	cmd_time,
	cmd_trigs,
	cmd_trigt,
	cmd_trigtonce,
	/* exit / quit */
	0,
	0
};
int num_args[]=
{
	/* data */
		/* bins */	2,
		/* cnt */	3,
		/* dd */	1,
		/* dindex */	0,
		/* disp */	3,
		/* dt */	0,
		/* flt */	3,
		/* fltintf */	5,
		/* index */	2,
		/* minmax */	1,
		/* receive */	0,
		/* sd */	0,
		/* send */	0,
		/* tag */	2,
	/* file */
		/* load */	2,
		/* loadtext */	2,
		/* save */	2,
		/* savetext */	2,
		/* xy */	3,
	/* list */
		/* dl */	2,
		/* sl */	1,
	/* loop */
		/* lp */	3,
		/* lpseed */	5,
		/* randinit */	3,
		/* randinitL */	3,
		/* randinitU */	3,
		/* tsc */	2,
	/* plot */
		/* anrt */	15,
		/* anstep */	9,
		/* clspins */	4,
		/* spins */	5,
		/* spinsfix */	5,
	/* run */
		/* rundiag */	2,
		/* runfixper */	2,
		/* runperper */	2,
	/* save */
		/* fbr */	5,
		/* fcl */	4,
		/* fe */	4,
		/* finalper */	4,
		/* fintf */	8,
		/* fm */	4,
		/* fmabs */	4,
		/* fperc */	7,
		/* fs */	4,
		/* ft */	4,
		/* incT */	3,
		/* svseed */	5,
	/* set */
		/* af */	2,
		/* blkinit */	2,
		/* block */	8,
		/* blockrev */	7,
		/* cpcur */	1,
		/* cpinit */	1,
		/* diag1 */	1,
		/* diag2 */	1,
		/* reset */	5,
		/* rnglcg */	1,
		/* rngmt */	1,
		/* seed */	3,
		/* setdiag */	9,
		/* setfixper */	8,
		/* setperper */	8,
		/* settsc */	1,
	/* step */
		/* energy */	4,
		/* equstop */	9,
		/* equtable */	11,
		/* flips */	4,
		/* mag */	4,
		/* movie1 */	5,
		/* movie2 */	7,
		/* movie3 */	7,
		/* slabstop */	10,
		/* stopdiag */	4,
		/* swstop */	6,
		/* time */	4,
		/* trigs */	5,
		/* trigt */	5,
		/* trigtonce */	5,
	/* exit / quit */
		/* exit */	0,
		/* quit */	0
};
int arg_types[][32]=
{
	/* data */
		/* bins */	{type_short_int, type_int},
		/* cnt */	{type_short_int, type_double, type_double},
		/* dd */	{type_short_int},
		/* dindex */	{},
		/* disp */	{type_short_int, type_int, type_int},
		/* dt */	{},
		/* flt */	{type_short_int, type_double, type_double},
		/* fltintf */	{type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* index */	{type_short_int, type_int},
		/* minmax */	{type_short_int},
		/* receive */	{},
		/* sd */	{},
		/* send */	{},
		/* tag */	{type_short_int, type_short_int},
	/* file */
		/* load */	{type_short_int, type_string},
		/* loadtext */	{type_short_int, type_string},
		/* save */	{type_short_int, type_string},
		/* savetext */	{type_short_int, type_string},
		/* xy */	{type_short_int, type_short_int, type_string},
	/* list */
		/* dl */	{type_short_int, type_short_int},
		/* sl */	{type_short_int},
	/* loop */
		/* lp */	{type_short_int, type_int, type_short_int},
		/* lpseed */	{type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* randinit */	{type_short_int, type_short_int, type_double},
		/* randinitL */	{type_short_int, type_short_int, type_double},
		/* randinitU */	{type_short_int, type_short_int, type_double},
		/* tsc */	{type_short_int, type_short_int},
	/* plot */
		/* anrt */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_double, type_double, type_double, type_double, type_double, type_short_int, type_string, type_string, type_string, type_string},
		/* anstep */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_double, type_double, type_string, type_string},
		/* clspins */	{type_short_int, type_int, type_string, type_string},
		/* spins */	{type_short_int, type_short_int, type_int, type_string, type_string},
		/* spinsfix */	{type_short_int, type_short_int, type_int, type_string, type_string},
	/* run */
		/* rundiag */	{type_short_int, type_short_int},
		/* runfixper */	{type_short_int, type_short_int},
		/* runperper */	{type_short_int, type_short_int},
	/* save */
		/* fbr */	{type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* fcl */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fe */	{type_short_int, type_short_int, type_short_int, type_int},
		/* finalper */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fintf */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_int, type_int},
		/* fm */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fmabs */	{type_short_int, type_short_int, type_short_int, type_int},
		/* fperc */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_int},
		/* fs */	{type_short_int, type_short_int, type_short_int, type_int},
		/* ft */	{type_short_int, type_short_int, type_short_int, type_int},
		/* incT */	{type_short_int, type_short_int, type_double},
		/* svseed */    {type_short_int, type_short_int, type_short_int, type_short_int, type_int},
	/* set */
		/* af */	{type_short_int, type_short_int},
		/* blkinit */	{type_short_int, type_int},
		/* block */	{type_short_int, type_short_int, type_int, type_int, type_int, type_int, type_int, type_int},
		/* blockrev */	{type_short_int, type_int, type_int, type_int, type_int, type_int, type_int},
		/* cpcur */	{type_short_int},
		/* cpinit */	{type_short_int},
		/* diag1 */	{type_short_int},
		/* diag2 */	{type_short_int},
		/* reset */	{type_short_int, type_unsigned_long_int, type_double, type_double, type_double},
		/* rnglcg */	{type_short_int},
		/* rngmt */	{type_short_int},
		/* seed */	{type_short_int, type_double, type_double},
		/* setdiag */	{type_short_int, type_short_int, type_int, type_int, type_unsigned_long_int, type_double, type_double, type_double, type_double},
		/* setfixper */	{type_short_int, type_short_int, type_int, type_int, type_unsigned_long_int, type_double, type_double, type_double},
		/* setperper */	{type_short_int, type_short_int, type_int, type_int, type_unsigned_long_int, type_double, type_double, type_double},
		/* settsc */	{type_short_int},
	/* step */
		/* energy */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* equstop */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_int, type_short_int, type_int},
		/* equtable */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_unsigned_long_int, type_double, type_int},
		/* flips */	{type_short_int, type_short_int, type_short_int, type_unsigned_long_int},
		/* mag */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* movie1 */	{type_short_int, type_short_int, type_short_int, type_int, type_short_int},
		/* movie2 */	{type_short_int, type_short_int, type_double, type_double, type_int, type_short_int, type_int},
		/* movie3 */	{type_short_int, type_short_int, type_double, type_double, type_int, type_short_int, type_int},
		/* slabstop */	{type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_short_int, type_int, type_double, type_short_int, type_int},
		/* stopdiag */	{type_short_int, type_short_int, type_short_int, type_double},
		/* swstop */	{type_short_int, type_short_int, type_short_int, type_short_int, type_double, type_int},
		/* time */	{type_short_int, type_short_int, type_short_int, type_short_int},
		/* trigs */	{type_short_int, type_short_int, type_short_int, type_unsigned_long_int, type_short_int},
		/* trigt */	{type_short_int, type_short_int, type_short_int, type_double, type_short_int},
		/* trigtonce */	{type_short_int, type_short_int, type_short_int, type_double, type_int},
	/* exit / quit */
		/* exit */	{},
		/* quit */	{}
};

