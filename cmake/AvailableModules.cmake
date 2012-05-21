# -*- cmake -*-
#
# List of available optional modules and their respective Mercurial repositories
#

# To add a new entry: (Also see the commented-out example.)
# 1: Create a new line in the AVAILABLE_NLS_MODULES list with a unique name.
# 2: Set up the repository using the following pattern:
#   set(NLS_MODULE_${RepoName}_REPOSITORY "") # The repository address from which the module can be cloned.
#   set(NLS_MODULE_${RepoName}_BRANCH "") # The branch to clone from.  If blank or not set, branch "default" is used.
#   set(NLS_MODULE_${RepoName}_CHANGESET "") # The changeset to clone from.  If blank or not set, tip is used.
#  Where "${RepoName}" is to be replaced by the unique name used in AVAILABLE_NLS_MODULES, making sure to match the case.

set(AVAILABLE_NLS_MODULES
	#MyExample
	RenderingSimple # Built-in module, no repository information needed.
	SoundSimple # Built-in module, no repository information needed.
	PhysicsSimple # Built-in module, no repository information needed.
	
	
	# Ignore the following line.  New modules should be listed above.
	CACHE STRING "List of available modules." FORCE
)

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#set(NLS_MODULE_MyExample_REPOSITORY "https://bitbucket.org/username/MyExample")
#set(NLS_MODULE_MyExample_BRANCH "Version 1")
#set(NLS_MODULE_MyExample_CHANGESET "409fd58af531")
