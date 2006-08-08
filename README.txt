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
             branches/users/username. Cannot write to branches/devel 
             nor trunk. The developer is the only person who may write 
             to their own branch in branches/users/username. A Trusted
             Developer may commit whatever they want to
             branches/users/username, though please remember that as
             this is a public repository, then anything you commit
             will be available to anyone who wishes to read it.
          
(4) User: Has only read-access to this repository.

In addition to the above, a single developer will be assigned to manage
each release of the code. Only that single developer 

All commits to this repository are monitored. Please only write to those
parts of the repository to which you have permission, or else your commits
will be reversed and you will be issued with a warning. If you repeatedly
write to forbidden areas then your repository privileges may be revoked.
