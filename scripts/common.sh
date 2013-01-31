######################################################################
#                            VARIABLES                               #
######################################################################
# Get vishnu version
versionNumber=$(awk -F'"' '/VISHNU_VERSION .*/ {gsub(/ /,"_", $2); print $2}' CMakeLists.txt)
versionComment=$(awk -F'"' '/VISHNU_VERSION_COMMENTS .*/ {gsub(/ /,"_", $2); print $2}' CMakeLists.txt)

NO_VERSION="${versionNumber}${versionComment}"


# get complete path to script dir
vishnuDir=$(pwd)
scriptDir=$vishnuDir/scripts

nbfailed=0
nbexec=0

######################################################################
#                           /VARIABLES                               #
######################################################################


######################################################################
#                            FUNCTIONS                               #
######################################################################
function run_cmd() {
    $@
    tmprv=$?
    (( nbexec++ ))
    if (( $tmprv != 0 )); then
        nbfailed=$(($nbfailed+1))
        echo "## Command failed: $@"
    fi
}

function run_cmd_noOutput() {
    $@  >/dev/null 2>&1
    tmprv=$?
    (( nbexec++ ))
    if (( $tmprv != 0 )); then
        (( nbfailed++ ))
        echo "## Command failed: $@"
    fi
}

function print_exit() {
    echo
    echo
    echo "## Failed / total: $nbfailed / $nbexec"
    
    exit $nbfailed
}


# Function that copy the dir in the archive and the depot copy
function copy_dir () {
    run_cmd cp -r $1 $path/$2;
    run_cmd cp -r $1 $pathrel/$2;
}

function copy_file () {
    run_cmd cp $1 $path/$2;
    run_cmd cp $1 $pathrel/$2;
}

# Function that create the dir in the archive and the depot copy
function create_dir () {
    run_cmd mkdir -p $path/$1;
    run_cmd mkdir -p $pathrel/$1;
}

function copy_rel_file () {
    run_cmd cp $1 $pathrel/$2;
}

function copy_rel_dir () {
    run_cmd cp -r $1 $pathrel/$2;
}

function copy_ar_file () {
    run_cmd cp $1 $path/$2;
}

function remove_files () {
    run_cmd rm -rf $@;
}

function change_dir () {
    run_cmd cd $1;
    location=$(pwd)
    echo "## Now in $location"
}
######################################################################
#                           /FUNCTIONS                               #
######################################################################
