-----------------------------------
Sire Subversion Repository Contents
-----------------------------------

branches
--------
Contains different branches of the code, e.g. development,
and user-specific branches.

branches/users
--------------
Contains branches that are owned by individual users.

branches/users/username
-----------------------
Contains the branch owned by the user with username 'username'. 
This user is the only person allowed to commit to this branch, and
is free to do whatever they wish within this branch.

branches/devel
--------------
Contains the active development of the code. Only code from 
branches/devel is allowed to be merged into trunk.

trunk
-----
Contains the latest development version of the code that is
known to compile, link, run, and passes the set of tests.

releases
--------
Contains specific releases of the code. Only code that has
been in trunk is allowed to be copied into a release.

releases/x
----------
Contains the data for release 'x' of the code, e.g. releases/0_1
would contain the data for version 0.1.

releases/x/trunk
----------------
Contains the latest stable development branch of release 'x'. Code
must pass through releases/x/trunk before being released.

releases/x/devel
----------------
Contains the development branch of release 'x'. This allows the release
to be developed so that further sub-releases may be made, e.g. bugfixes.
Only code from releases/x/devel may be merged into releases/x/trunk

releases/x/versions
-------------------
Contains each of the released sub-versions of release 'x', e.g. 0.1.3
would be contained in releases/0_1/versions/3, while 0.1.4alpha would
be in releases/0_1/versions/4alpha

--------------------
Repository Etiquette
--------------------

There are four types of developer on this project;

(1) Master: Has complete control, manages trunk and decides
            when to make a release.
            
(2) Core: Almost as powerful as Master. Can write to branches/devel
          but does not have the power to write to trunk nor decide
          when to make a release.
          
(3) Trusted: General developer who should only write to 
             branches/users/username. Cannot write to branches/devel,
	     releases nor trunk. The developer is the only person who may 
	     write to their own branch in branches/users/username. A Trusted
             Developer may commit whatever they want to
             branches/users/username, though please remember that as
             this is a public repository, then anything you commit
             will be available to anyone who wishes to read it.
          
(4) User: Has only read-access to this repository.

In addition to the above, a single developer will be assigned to manage
each release of the code. Only that single developer will have write access 
to the versions branch and the trunk of that release (e.g. only
the release manager for version 0.2 will have write access to 
releases/0_2/versions and releases/0_2/trunk).

All commits to this repository are monitored. Please only write to those
parts of the repository to which you have permission, or else your commits
will be reversed and you will be issued with a warning. If you repeatedly
write to forbidden areas then your repository privileges may be revoked.

Also, the total size of this repository is limited to 100MB. Please avoid
adding large files to this repository unless you really have to! The
developers are not likely to allow large files to be included in any
releases unless they are really necessary. If you wish to include large
data files then please first post a request on the sire-developers mailing
list and we will discuss it with you.

-----------------
Release Numbering
-----------------

No releases will be made that fail to compile or fail to pass the defined
set of tests. The release numbers use a standard three-part identifier;

x.y.z

"x" is the major release number. The API and binary data formats of all
versions that have the same major number should be backwards-compatible.
(e.g. binary files or scripts written for 1.0.0 should work with all
versions up to 1.9).

"y" is the minor release number. This corresponds to a new version of the 
code within the same major release. Each new minor release will contain 
new features, and possibly significantly new components and modules. The
aim will be to have a new minor release at periodic intervals, e.g. every
six months.

"z" is the minor revision number. This corresponds to a new version of
a particular release. Each minor revision should contain only bugfixes
and/or backports from later code. This will allow a release to be maintained
as long as there are users who wish to continue using it.

So, for example, version 1.2.5 corresponds to the fifth revision of the 
second release of the 1.x series.

In addition to release numbers, there may be additional qualifications,
e.g. alpha, beta, rc (release candidate) etc. These may be used
to allow more complete testing of a potential release before it is
finally tagged and made public.

Each release will be found in its own branch in the releases directory
of the repository. Each major-minor version has its own directory,
e.g. 1.2.5 will be in the directory releases/1_2

Within this directory there will be three sub-directories; 
trunk, devel and versions.

trunk is used to keep the latest stable development version of this 
major-minor release. devel is used to store the latest, possible unstable
version of this major-minor release. versions stores each of the tagged
versions of this release, e.g. 1.2.5 will be in the directory
releases/1_2/versions/5, while 1.2.6beta1 will be in
releases/1_2/versions/6beta1

