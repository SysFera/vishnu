#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/process/all.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include "tmsTestUtils.hpp"
#include "utilVishnu.hpp"
#include "tmsUtils.hpp"

using namespace std;
namespace bp = boost::process;

namespace ba = boost::assign;
namespace bfs = boost::filesystem;
namespace bs = boost::system;


static std::string torque_Fast_Script = std::string("#!/bin/sh\n")+
                                        "#PBS -N  sequentiel\n"+
                                        "#PBS -l walltime=00:02:07\n"+
                                        "#PBS -q second_queue\n" +
                                        "echo \"Queue name: \" $PBS_O_QUEUE \n"+
                                        "echo \"Server Name: \" $PBS_SERVER\n"+
                                        "echo \"Working dir: \" $PBS_O_WORKDIR\n"+
                                        "echo \"PBS_O_HOST: \" $PBS_O_HOST\n"+
                                        "echo \"PBS_ARRAYID: \" $PBS_ARRAYID\n"+
                                        "echo \"PBS_ENVIRONMENT :\" $PBS_ENVIRONMENT\n"+
                                        "echo \"PBS_O_HOME: \" $PBS_O_HOME\n"+
                                        "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                        "echo \"#TEST_JOB_ID: \"$VISHNU_BATCHJOB_ID\n"+
                                        "echo \"#TEST_JOB_NAME: \" $VISHNU_BATCHJOB_NAME\n"+
                                        "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                        "echo \"#TEST_NUM_NODES: \"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                        "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                        "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                        "echo \"#BATCH_JOB_ID: \"$PBS_JOBID\n"+
                                        "echo \"#BATCH_JOB_NAME: \" $PBS_JOBNAME\n"+
                                        "echo \"#BATCH_NODEFILE:\" $PBS_NODEFILE\n"+
                                        "echo \"#BATCH_NUM_NODES:\"\n"+
                                        "sleep 10 ";

static std::string torque_Wait_Script = std::string("#!/bin/sh\n")+
                                        "#PBS -N  sequentiel\n"+
                                        "#PBS -l walltime=00:02:07\n"+
                                        "#PBS -q second_queue\n" +
                                        "echo \"Queue name: \" $PBS_O_QUEUE \n"+
                                        "echo \"Server Name: \" $PBS_SERVER\n"+
                                        "echo \"Working dir: \" $PBS_O_WORKDIR\n"+
                                        "echo \"PBS_O_HOST: \" $PBS_O_HOST\n"+
                                        "echo \"PBS_ARRAYID: \" $PBS_ARRAYID\n"+
                                        "echo \"PBS_ENVIRONMENT :\" $PBS_ENVIRONMENT\n"+
                                        "echo \"PBS_O_HOME: \" $PBS_O_HOME\n"+
                                        "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                        "echo \"#TEST_JOB_ID: \"$VISHNU_BATCHJOB_ID\n"+
                                        "echo \"#TEST_JOB_NAME: \" $VISHNU_BATCHJOB_NAME\n"+
                                        "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                        "echo \"#TEST_NUM_NODES: \"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                        "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                        "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                        "echo \"#BATCH_JOB_ID: \"$PBS_JOBID\n"+
                                        "echo \"#BATCH_JOB_NAME: \" $PBS_JOBNAME\n"+
                                        "echo \"#BATCH_NODEFILE:\" $PBS_NODEFILE\n"+
                                        "echo \"#BATCH_NUM_NODES:\"\n"+
                                        "sleep 300 ";
static std::string torque_output_Script = std::string("#!/bin/sh\n")+
                                          "#PBS -N  sequentiel\n"+
                                          "#PBS -l walltime=00:02:07\n"+
                                          "#PBS -q second_queue\n" +
                                          "echo \"Queue name: \" $PBS_O_QUEUE \n"+
                                          "echo \"Server Name: \" $PBS_SERVER\n"+
                                          "echo \"Working dir: \" $PBS_O_WORKDIR\n"+
                                          "echo \"PBS_O_HOST: \" $PBS_O_HOST\n"+
                                          "echo \"PBS_ARRAYID: \" $PBS_ARRAYID\n"+
                                          "echo \"PBS_ENVIRONMENT :\" $PBS_ENVIRONMENT\n"+
                                          "echo \"PBS_O_HOME: \" $PBS_O_HOME\n"+
                                          "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                          "echo \"#TEST_JOB_ID: \"$VISHNU_BATCHJOB_ID\n"+
                                          "echo \"#TEST_JOB_NAME: \" $VISHNU_BATCHJOB_NAME\n"+
                                          "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                          "echo \"#TEST_NUM_NODES: \"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                          "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                          "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                          "echo \"#BATCH_JOB_ID: \"$PBS_JOBID\n"+
                                          "echo \"#BATCH_JOB_NAME: \" $PBS_JOBNAME\n"+
                                          "echo \"#BATCH_NODEFILE:\" $PBS_NODEFILE\n"+
                                          "echo \"#BATCH_NUM_NODES:\"\n"+
                                          "echo \"Output directory test\" > $VISHNU_OUTPUT_DIR/TMS_res\n"+
                                          "sleep 300 \n";
static std::string torque_Bad_Script = std::string("#!/bin/sh\n")+
                                       "#PBS -q badQueueWWQQCCQCQXQ\n";
static std::string slurm_Fast_Script = std::string("#!/bin/sh\n")+
                                       "#SBATCH -J myFristJob\n"+
                                       "#SBATCH -o myJob-%J.out\n"+
                                       "#SBATCH -e myJob-%J.err\n"+
                                       "#SBATCH -t 00:00:08\n"+
                                       "#SBATCH --comment=\"my first slurm job\"\n"+
                                       "#SBATCH --share\n\n"+
                                       "srun hostname\n"+
                                       "sleep 10\n"+
                                       "echo \"Hello world!....\"\n"+
                                       "echo \"Your job submit directory is: $SLURM_SUBMIT_DIR\" \n"+
                                       "echo \"The list of nodes allocated to the job: $SLURM_JOB_NODELIST\" \n"+
                                       "echo \"The count of processors available to the job on this node: $SLURM_JOB_CPUS_PER_NODE\" \n"+
                                       "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"   \n"+
                                       "echo \"#TEST_JOB_ID: \"$VISHNU_BATCHJOB_ID                      \n"+
                                       "echo \"#TEST_JOB_NAME: \" $VISHNU_BATCHJOB_NAME                 \n"+
                                       "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE              \n"+
                                       "cat $VISHNU_BATCHJOB_NODEFILE                                   \n"+
                                       "echo \"#TEST_NUM_NODES: \"$VISHNU_BATCHJOB_NUM_NODES            \n"+
                                       "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME \n"+
                                       "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!....\"    \n"+
                                       "echo \"#BATCH_JOB_ID   : $SLURM_JOB_ID\"                        \n"+
                                       "echo \"#BATCH_JOB_NAME : $SLURM_JOB_NAME\"                      \n"+
                                       "echo \"#BATCH_NODEFILE : $SLURM_JOB_NODELIST\"                  \n"+
                                       "echo \"#BATCH_NUM_NODES: $SLURM_JOB_NUM_NODES\"                 \n";
static std::string slurm_Wait_Script = std::string("#!/bin/sh\n")+
                                       "#SBATCH -J myFristJob\n"+
                                       "#SBATCH -o myJob-%J.out\n"+
                                       "#SBATCH -e myJob-%J.err\n"+
                                       "#SBATCH -t 00:00:08\n"+
                                       "#SBATCH --comment=\"my first slurm job\"\n"+
                                       "#SBATCH --share\n\n"+
                                       "srun hostname\n"+
                                       "sleep 300\n"+
                                       "echo \"Hello world!....\"\n"+
                                       "echo \"Your job submit directory is: $SLURM_SUBMIT_DIR\" \n"+
                                       "echo \"The list of nodes allocated to the job: $SLURM_JOB_NODELIST\" \n"+
                                       "echo \"The count of processors available to the job on this node: $SLURM_JOB_CPUS_PER_NODE\" \n"+
                                       "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"   \n"+
                                       "echo \"#TEST_JOB_ID: \"$VISHNU_BATCHJOB_ID                      \n"+
                                       "echo \"#TEST_JOB_NAME: \" $VISHNU_BATCHJOB_NAME                 \n"+
                                       "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE              \n"+
                                       "cat $VISHNU_BATCHJOB_NODEFILE                                   \n"+
                                       "echo \"#TEST_NUM_NODES: \"$VISHNU_BATCHJOB_NUM_NODES            \n"+
                                       "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME \n"+
                                       "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!....\"    \n"+
                                       "echo \"#BATCH_JOB_ID   : $SLURM_JOB_ID\"                        \n"+
                                       "echo \"#BATCH_JOB_NAME : $SLURM_JOB_NAME\"                      \n"+
                                       "echo \"#BATCH_NODEFILE : $SLURM_JOB_NODELIST\"                  \n"+
                                       "echo \"#BATCH_NUM_NODES: $SLURM_JOB_NUM_NODES\"                 \n";
static std::string slurm_output_Script = std::string("#!/bin/sh\n")+
                                         "#SBATCH -J myFristJob\n"+
                                         "#SBATCH -o myJob-%J.out\n"+
                                         "#SBATCH -e myJob-%J.err\n"+
                                         "#SBATCH -t 00:00:08\n"+
                                         "#SBATCH --comment=\"my first slurm job\"\n"+
                                         "#SBATCH --share\n\n"+
                                         "echo \"Hello world!....\"\n"+
                                         "echo \"Your job submit directory is: $SLURM_SUBMIT_DIR\" \n"+
                                         "echo \"The list of nodes allocated to the job: $SLURM_JOB_NODELIST\" \n"+
                                         "echo \"The count of processors available to the job on this node: $SLURM_JOB_CPUS_PER_NODE\" \n"+
                                         "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"   \n"+
                                         "echo \"#TEST_JOB_ID: \"$VISHNU_BATCHJOB_ID                      \n"+
                                         "echo \"#TEST_JOB_NAME: \" $VISHNU_BATCHJOB_NAME                 \n"+
                                         "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE              \n"+
                                         "cat $VISHNU_BATCHJOB_NODEFILE                                   \n"+
                                         "echo \"#TEST_NUM_NODES: \"$VISHNU_BATCHJOB_NUM_NODES            \n"+
                                         "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME \n"+
                                         "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!....\"    \n"+
                                         "echo \"#BATCH_JOB_ID   : $SLURM_JOB_ID\"                        \n"+
                                         "echo \"#BATCH_JOB_NAME : $SLURM_JOB_NAME\"                      \n"+
                                         "echo \"#BATCH_NODEFILE : $SLURM_JOB_NODELIST\"                  \n"+
                                         "echo \"#BATCH_NUM_NODES: $SLURM_JOB_NUM_NODES\"                 \n"
                                         "echo \"Output directory test\" > $VISHNU_OUTPUT_DIR/TMS_res     \n";
static std::string slurm_Bad_Script = std::string("#!/bin/sh\n")+
                                      "#SBATCH --badcontent badValue";
static std::string lsf_Fast_Script = std::string("#!/bin/sh\n")+
                                     "#BSUB -J myLSFjobName\n"+
                                     "#BSUB -o myLSFJob-%J.out\n"+
                                     "#BSUB -e myLSFJob-%J.err\n"+
                                     "#BSUB -W 00:01:00\n"+
                                     "#BSUB -Jd \"my LSF job\"\n"+
                                     "sleep 10\n"+
                                     "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                     "echo \"#TEST_JOB_ID:\"$VISHNU_BATCHJOB_ID\n"+
                                     "echo \"#TEST_JOB_NAME:\" $VISHNU_BATCHJOB_NAME\n"+
                                     "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                     "cat $VISHNU_BATCHJOB_NODEFILE\n"+
                                     "echo \"#TEST_NUM_NODES:\"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                     "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                     "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                     "echo \"#BATCH_JOB_ID:\"$LSB_JOBID\n"+
                                     "echo \"#BATCH_JOB_NAME:\" $LSB_JOBNAME\n";

static std::string lsf_Wait_Script = std::string("#!/bin/sh\n")+
                                     "#BSUB -J myLSFjobName\n"+
                                     "#BSUB -o myLSFJob-%J.out\n"+
                                     "#BSUB -e myLSFJob-%J.err\n"+
                                     "#BSUB -W 00:01:00\n"+
                                     "#BSUB -Jd \"my LSF job\"\n"+
                                     "sleep 300\n"+
                                     "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                     "echo \"#TEST_JOB_ID:\"$VISHNU_BATCHJOB_ID\n"+
                                     "echo \"#TEST_JOB_NAME:\" $VISHNU_BATCHJOB_NAME\n"+
                                     "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                     "cat $VISHNU_BATCHJOB_NODEFILE\n"+
                                     "echo \"#TEST_NUM_NODES:\"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                     "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                     "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                     "echo \"#BATCH_JOB_ID:\"$LSB_JOBID\n"+
                                     "echo \"#BATCH_JOB_NAME:\" $LSB_JOBNAME\n";
static std::string lsf_output_Script = std::string("#!/bin/sh\n")+
                                       "#BSUB -J myLSFjobName\n"+
                                       "#BSUB -o myLSFJob-%J.out\n"+
                                       "#BSUB -e myLSFJob-%J.err\n"+
                                       "#BSUB -W 00:01:00\n"+
                                       "#BSUB -Jd \"my LSF job\"\n"+
                                       "sleep 10\n"+
                                       "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                       "echo \"#TEST_JOB_ID:\"$VISHNU_BATCHJOB_ID\n"+
                                       "echo \"#TEST_JOB_NAME:\" $VISHNU_BATCHJOB_NAME\n"+
                                       "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                       "cat $VISHNU_BATCHJOB_NODEFILE\n"+
                                       "echo \"#TEST_NUM_NODES:\"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                       "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                       "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                       "echo \"#BATCH_JOB_ID:\"$LSB_JOBID\n"+
                                       "echo \"#BATCH_JOB_NAME:\" $LSB_JOBNAME\n"+
                                       "echo \"Output directory test\" > $VISHNU_OUTPUT_DIR/TMS_res\n";
static std::string lsf_Bad_Script = std::string("#!/bin/sh\n")+
                                    "#BSUB -Q abc\n";
static std::string sge_Fast_Script = std::string("#!/bin/sh\n")+
                                     "#$ -N mySGEjobName\n"+
                                     "#$ -o mySGEJob-$JOB_ID.out\n"+
                                     "#$ -e mySGEJob-$JOB_ID.err\n"+
                                     "sleep 10\n"+
                                     "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!\"\n"+
                                     "echo \"#TEST_JOB_ID:\"$VISHNU_BATCHJOB_ID\n"+
                                     "echo \"#TEST_JOB_NAME:\" $VISHNU_BATCHJOB_NAME\n"+
                                     "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                     "cat $VISHNU_BATCHJOB_NODEFILE"+
                                     "echo \"#TEST_NUM_NODES:\"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                     "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                     "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!\"\n"+
                                     "echo \"#BATCH_JOB_ID:\"$JOB_ID\n"+
                                     "echo \"#BATCH_JOB_NAME: \" $JOB_NAME\n"+
                                     "echo \"VISHNU_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n";
static std::string sge_Wait_Script = std::string("#!/bin/sh\n")+
                                     "#$ -N mySGEjobName\n"+
                                     "#$ -o mySGEJob-$JOB_ID.out\n"+
                                     "#$ -e mySGEJob-$JOB_ID.err\n"+
                                     "sleep 300\n"+
                                     "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!\"\n"+
                                     "echo \"#TEST_JOB_ID:\"$VISHNU_BATCHJOB_ID\n"+
                                     "echo \"#TEST_JOB_NAME:\" $VISHNU_BATCHJOB_NAME\n"+
                                     "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                     "cat $VISHNU_BATCHJOB_NODEFILE"+
                                     "echo \"#TEST_NUM_NODES:\"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                     "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                     "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!\"\n"+
                                     "echo \"#BATCH_JOB_ID:\"$JOB_ID\n"+
                                     "echo \"#BATCH_JOB_NAME: \" $JOB_NAME\n"+
                                     "echo \"VISHNU_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n";
static std::string sge_output_Script = std::string("#!/bin/sh\n")+
                                       "#$ -N mySGEjobName\n"+
                                       "#$ -o mySGEJob-$JOB_ID.out\n"+
                                       "#$ -e mySGEJob-$JOB_ID.err\n"+
                                       "sleep 10\n"+
                                       "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!\"\n"+
                                       "echo \"#TEST_JOB_ID:\"$VISHNU_BATCHJOB_ID\n"+
                                       "echo \"#TEST_JOB_NAME:\" $VISHNU_BATCHJOB_NAME\n"+
                                       "echo \"#TEST_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                       "cat $VISHNU_BATCHJOB_NODEFILE"+
                                       "echo \"#TEST_NUM_NODES:\"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                       "echo \"#TEST_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                       "echo \"CORRESPONDING JOB OUTPUT ENVIRONMENT VARIABLES!\"\n"+
                                       "echo \"#BATCH_JOB_ID:\"$JOB_ID\n"+
                                       "echo \"#BATCH_JOB_NAME: \" $JOB_NAME\n"+
                                       "echo \"VISHNU_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                       "echo \"Output directory test\" > $VISHNU_OUTPUT_DIR/TMS_res\n";
static std::string sge_Bad_Script = std::string("#!/bin/sh\n") + "#$ -q abc";
static std::string ll_Fast_Script = "";
static std::string ll_Wait_Script = "";
static std::string ll_output_Script = "";
static std::string ll_Bad_Script = "";
static std::string generic_Fast_Script = std::string("#!/bin/sh\n")+
                                         "#BSUB -J first_job\n"+
                                         "#% vishnu_job_name=first_job\n"+
                                         "#% vishnu_output=my_first_job_gen.out\n"+
                                         "#% vishnu_error=my_first_job_gen.err\n"+
                                         "#% vishnu_mailNotification= BEGIN\n"+
                                         "#% vishnu_wallclocklimit=01:00:00\n"+
                                         "# PBS -l nodes=2:ppn=1+1+1:ppn=2\n"+
                                         "#SBATCH --comment \"Test job with vishnu generic script\""+
                                         "# PBS -l walltime=01:00:00\n"+
                                         "#SBATCH -p firstPart\n"+
                                         "#BSUB -q priority\n"+
                                         "#$ -N mySGEjobName\n"+
                                         "#$ -o mySGEJob-$JOB_ID.out\n"+
                                         "#$ -e mySGEJob-$JOB_ID.err\n"+
                                         "echo \"The name of the submitted server is: \" $HOSTNAME\n"+
                                         "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                         "echo \"VISHNU_BATCHJOB_ID: \"$VISHNU_BATCHJOB_ID\n"+
                                         "echo \"VISHNU_BATCHJOB_NAME: \" $VISHNU_BATCHJOB_NAME\n"+
                                         "echo \"VISHNU_BATCHJOB_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                         "cat $VISHNU_BATCHJOB_NODEFILE\n"+
                                         "echo \"VISHNU_BATCHJOB_NUM_NODES: \"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                         "echo \"VISHNU_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                         "sleep 10\n";
static std::string generic_Wait_Script = std::string("#!/bin/sh\n")+
                                         "#BSUB -J first_job\n"+
                                         "#% vishnu_job_name=first_job\n"+
                                         "#% vishnu_output=my_first_job_gen.out\n"+
                                         "#% vishnu_error=my_first_job_gen.err\n"+
                                         "#% vishnu_mailNotification= BEGIN\n"+
                                         "#% vishnu_wallclocklimit=01:00:00\n"+
                                         "# PBS -l nodes=2:ppn=1+1+1:ppn=2\n"+
                                         "#SBATCH --comment \"Test job with vishnu generic script\"\n"+
                                         "# PBS -l walltime=01:00:00\n"+
                                         "#SBATCH -p firstPart\n"+
                                         "#BSUB -q priority\n"+
                                         "#$ -N mySGEjobName\n"+
                                         "#$ -o mySGEJob-$JOB_ID.out\n"+
                                         "#$ -e mySGEJob-$JOB_ID.err\n"+
                                         "echo \"The name of the submitted server is: \" $HOSTNAME\n"+
                                         "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                         "echo \"VISHNU_BATCHJOB_ID: \"$VISHNU_BATCHJOB_ID\n"+
                                         "echo \"VISHNU_BATCHJOB_NAME: \" $VISHNU_BATCHJOB_NAME\n"+
                                         "echo \"VISHNU_BATCHJOB_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                         "cat $VISHNU_BATCHJOB_NODEFILE\n"+
                                         "echo \"VISHNU_BATCHJOB_NUM_NODES: \"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                         "echo \"VISHNU_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                         "sleep 300\n";
static std::string generic_Output_Script = std::string("#!/bin/sh\n")+
                                           "#BSUB -J first_job\n"+
                                           "#% vishnu_job_name=first_job\n"+
                                           "#% vishnu_output=my_first_job_gen.out\n"+
                                           "#% vishnu_error=my_first_job_gen.err\n"+
                                           "#% vishnu_mailNotification= BEGIN\n"+
                                           "#% vishnu_wallclocklimit=01:00:00\n"+
                                           "# PBS -l nodes=2:ppn=1+1+1:ppn=2\n"+
                                           "#SBATCH --comment \"Test job with vishnu generic script\"\n"+
                                           "# PBS -l walltime=01:00:00\n"+
                                           "#SBATCH -p firstPart\n"+
                                           "#BSUB -q priority\n"+
                                           "#$ -N mySGEjobName\n"+
                                           "#$ -o mySGEJob-$JOB_ID.out\n"+
                                           "#$ -e mySGEJob-$JOB_ID.err\n"+
                                           "echo \"The name of the submitted server is: \" $HOSTNAME\n"+
                                           "echo \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"\n"+
                                           "echo \"VISHNU_BATCHJOB_ID: \"$VISHNU_BATCHJOB_ID\n"+
                                           "echo \"VISHNU_BATCHJOB_NAME: \" $VISHNU_BATCHJOB_NAME\n"+
                                           "echo \"VISHNU_BATCHJOB_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\n"+
                                           "cat $VISHNU_BATCHJOB_NODEFILE\n"+
                                           "echo \"VISHNU_BATCHJOB_NUM_NODES: \"$VISHNU_BATCHJOB_NUM_NODES\n"+
                                           "echo \"VISHNU_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\n"+
                                           "sleep 10\n";
static std::string generic_Bad_Script = "";




VishnuConnexion::VishnuConnexion(const string& uid, const string& upwd, const UMS_Data::ConnectOptions& co):muid(uid),mupwd(upwd),mco(co),open(false){

}

VishnuConnexion::~VishnuConnexion(){
  if (true==open) {
    try {
      BOOST_REQUIRE_EQUAL(vishnu::close(msession.getSessionKey()),0);
      BOOST_TEST_MESSAGE("The session is closed");
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}

string VishnuConnexion::getConnexion(){
  try {
    BOOST_REQUIRE(vishnu::connect(muid, mupwd, msession, mco) == 0);
    open=true;
    BOOST_TEST_MESSAGE("The session is open");
    return msession.getSessionKey();
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
  return "";
}


bool operator== (const TMS_Data::Job& lJob,const TMS_Data::Job& rJob ){

  return ( (lJob.getId() == rJob.getId())
           && (lJob.getMachine() == rJob.getMachine())
           && (lJob.getPath() == rJob.getPath())
           && (lJob.getName() == rJob.getName())
           && (lJob.getPriority() == rJob.getPriority())
           && (lJob.getLocalAccount() == rJob.getLocalAccount() )
           && (lJob.getQueue() == rJob.getQueue() )
           && (lJob.getWallClockLimit() == rJob.getWallClockLimit() )


           );



}

std::string findValue(const std::string& content, const std::string& key) {

  size_t pos = content.rfind(key);
  std::string tmp = content.substr(pos+key.size());
  std::istringstream iss(tmp);
  std::string value;
  iss >> value;
  return value;
}

std::string
getFileContent(const std::string& filePath){

  bfs::path file (filePath);

  // Check the existence of file
  if (((false==bfs::exists(file)) || (true==bfs::is_empty(file)))
      || (false==bfs::is_regular_file(file))) {
    throw UserException(ERRCODE_INVALID_PARAM, "can not read the file: " + filePath);
  }

  bfs::ifstream ifs (file);

  // Read the whole file into string

  std::stringstream ss;
  ss << ifs.rdbuf();

  return ss.str();

}

std::string generateTmpScript(std::string& batchtype, std::string scriptType){



  bfs::path tempdir = boost::filesystem::temp_directory_path();

  std::string scriptpath = tempdir.string() + "/test_scriptXXXXXX";
  char* scriptchar = strdup(scriptpath.c_str());
  std::string content;

  if(batchtype == "TORQUE")
  {
    if (scriptType == "fast"){
      content = torque_Fast_Script;
    } else if (scriptType == "bad"){
      content = torque_Bad_Script;
    } else if (scriptType == "wait"){
      content = torque_Wait_Script;
    } else if (scriptType == "output"){
      content = torque_output_Script;
    }
  } else if (batchtype == "SLURM"){
    if (scriptType == "fast"){
      content = slurm_Fast_Script;
    } else if (scriptType == "bad"){
      content = slurm_Bad_Script;
    } else if (scriptType == "wait"){
      content = slurm_Wait_Script;
    } else if (scriptType == "output"){
      content = slurm_output_Script;
    }

  } else if (batchtype == "LSF"){
    if (scriptType == "fast"){
      content = lsf_Fast_Script;
    } else if (scriptType == "bad"){
      content = lsf_Bad_Script;
    } else if (scriptType == "wait"){
      content = lsf_Wait_Script;
    } else if (scriptType == "output"){
      content = lsf_output_Script;
    }
  } else if (batchtype == "SGE"){
    if (scriptType == "fast"){
      content = sge_Fast_Script;
    } else if (scriptType == "bad"){
      content = sge_Bad_Script;
    } else if (scriptType == "wait"){
      content = sge_Wait_Script;
    }else if (scriptType == "output"){
      content = sge_output_Script;
    }

  } else if (batchtype == "LL"){
    if (scriptType == "fast"){
      content = ll_Fast_Script;
    } else if (scriptType == "bad"){
      content = ll_Bad_Script;
    } else if (scriptType == "wait"){
      content = ll_Wait_Script;
    } else if (scriptType == "output"){
      content = ll_output_Script;
    }
    // TODO :
    // WTF no default case ? What is this 'generic' BS type ?
    // Where is posix batch case ?

  } else {
    if (scriptType == "fast"){
      content = generic_Fast_Script;
    } else if (scriptType == "bad"){
      content = generic_Bad_Script;
    } else if (scriptType == "wait"){
      content = generic_Wait_Script;
    } else if (scriptType == "output"){
      content = generic_Output_Script;
    }

  }
  vishnu::saveInFile(scriptchar, content);
std:string ret(scriptchar);

  return ret;

}
