This directory contains links to the Multi-scale Coarse-graining (MS-CG) library which is 
required to use the MS-CG package and its fix commands in a LAMMPS input script.

The MS-CG library is available at https://github.com/uchicago-voth/MSCG-release and was
developed by Jacob Wagner in Gregory Voth's group at the University of Chicago.

You must perform the following steps yourself.

1.  Download MS-CG at https://github.com/uchicago-voth/MSCG-release
    either as a tarball or via SVN, and unpack the
    tarball either in this /lib/mscg directory
    or somewhere else on your system.

2.  compile MS-CG from within its home directory using your makefile choice
    % make -f Makefile."name" lib_mscg.a
    It is recommended that you start from Makefile.g++_simple or Makefile.intel_simple

3.  There is no need to install MS-CG if you only wish
    to use it from LAMMPS. 

4.  Create two soft links in this dir (lib/ms-cg)
    to the MS-CG src directory is.  
    e.g if you built MS-CG in this dir:
      % ln -s mscgfm-master/src includelink
      % ln -s mscgfm-master/src liblink
    Note that these links could also be set to the include and lib
    directories created by a MS-CG install, e.g.
      % ln -s /usr/local/include includelink
      % ln -s /usr/local/lib liblink

When these steps are complete you can build LAMMPS
with the MS-CG package installed:

% cd lammps/src
% make yes-USER-MSCG
% make g++ (or whatever target you wish)

Note that if you download and unpack a new LAMMPS tarball,
the "includelink" and "liblink" files will be lost
and you will need to re-create them (step 4).  If you
built MS-CG in this directory (as opposed to somewhere
else on your system) and did not install it somewhere
else, you will also need to repeat steps 1,2,3.

The Makefile.lammps file in this directory is there for compatibility
with the way other libraries under the lib dir are linked with by
LAMMPS.  However, MS-CG requires no auxiliary files or
settings, so its variables are blank.
