/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sysfera.vishnu.api.vishnu.internal;

public class VISHNU {
  public static int connect(String userId, String password, Session session, ConnectOptions connectOpt) throws InternalUMSException {
    return VISHNUJNI.connect__SWIG_0(userId, password, Session.getCPtr(session), session, ConnectOptions.getCPtr(connectOpt), connectOpt);
  }

  public static int connect(ListUsers users, Session session, ConnectOptions connectOpt) throws InternalUMSException {
    return VISHNUJNI.connect__SWIG_1(ListUsers.getCPtr(users), users, Session.getCPtr(session), session, ConnectOptions.getCPtr(connectOpt), connectOpt);
  }

  public static int reconnect(String userId, String password, String sessionId, Session session) throws InternalUMSException {
    return VISHNUJNI.reconnect__SWIG_0(userId, password, sessionId, Session.getCPtr(session), session);
  }

  public static int reconnect(ListUsers users, String sessionId, Session session) throws InternalUMSException {
    return VISHNUJNI.reconnect__SWIG_1(ListUsers.getCPtr(users), users, sessionId, Session.getCPtr(session), session);
  }

  public static int close(String sessionKey) throws InternalUMSException {
    return VISHNUJNI.close(sessionKey);
  }

  public static int addUser(String sessionKey, User newUser) throws InternalUMSException {
    return VISHNUJNI.addUser(sessionKey, User.getCPtr(newUser), newUser);
  }

  public static int updateUser(String sessionKey, User user) throws InternalUMSException {
    return VISHNUJNI.updateUser(sessionKey, User.getCPtr(user), user);
  }

  public static int deleteUser(String sessionKey, String userId) throws InternalUMSException {
    return VISHNUJNI.deleteUser(sessionKey, userId);
  }

  public static int changePassword(String userId, String password, String passwordNew) throws InternalUMSException {
    return VISHNUJNI.changePassword(userId, password, passwordNew);
  }

  public static int resetPassword(String sessionKey, String userId, java.lang.String[] tmpPassword) throws InternalUMSException {
    return VISHNUJNI.resetPassword(sessionKey, userId, tmpPassword);
  }

  public static int addMachine(String sessionKey, Machine newMachine) throws InternalUMSException {
    return VISHNUJNI.addMachine(sessionKey, Machine.getCPtr(newMachine), newMachine);
  }

  public static int updateMachine(String sessionKey, Machine machine) throws InternalUMSException {
    return VISHNUJNI.updateMachine(sessionKey, Machine.getCPtr(machine), machine);
  }

  public static int deleteMachine(String sessionKey, String machineId) throws InternalUMSException {
    return VISHNUJNI.deleteMachine(sessionKey, machineId);
  }

  public static int addLocalAccount(String sessionKey, LocalAccount newLocalAccount, java.lang.String[] sshPublicKey) throws InternalUMSException {
    return VISHNUJNI.addLocalAccount(sessionKey, LocalAccount.getCPtr(newLocalAccount), newLocalAccount, sshPublicKey);
  }

  public static int updateLocalAccount(String sessionKey, LocalAccount localAccount) throws InternalUMSException {
    return VISHNUJNI.updateLocalAccount(sessionKey, LocalAccount.getCPtr(localAccount), localAccount);
  }

  public static int deleteLocalAccount(String sessionKey, String userId, String machineId) throws InternalUMSException {
    return VISHNUJNI.deleteLocalAccount(sessionKey, userId, machineId);
  }

  public static int saveConfiguration(String sessionKey, Configuration config) throws InternalUMSException {
    return VISHNUJNI.saveConfiguration(sessionKey, Configuration.getCPtr(config), config);
  }

  public static int restoreConfiguration(String sessionKey, String filePath) throws InternalUMSException {
    return VISHNUJNI.restoreConfiguration(sessionKey, filePath);
  }

  public static int configureOption(String sessionKey, OptionValue optionValue) throws InternalUMSException {
    return VISHNUJNI.configureOption(sessionKey, OptionValue.getCPtr(optionValue), optionValue);
  }

  public static int configureDefaultOption(String sessionKey, OptionValue optionValue) throws InternalUMSException {
    return VISHNUJNI.configureDefaultOption(sessionKey, OptionValue.getCPtr(optionValue), optionValue);
  }

  public static int listSessions(String sessionKey, ListSessions listSession, ListSessionOptions options) throws InternalUMSException {
    return VISHNUJNI.listSessions(sessionKey, ListSessions.getCPtr(listSession), listSession, ListSessionOptions.getCPtr(options), options);
  }

  public static int listLocalAccounts(String sessionKey, ListLocalAccounts listLocalAcc, ListLocalAccOptions options) throws InternalUMSException {
    return VISHNUJNI.listLocalAccounts(sessionKey, ListLocalAccounts.getCPtr(listLocalAcc), listLocalAcc, ListLocalAccOptions.getCPtr(options), options);
  }

  public static int listMachines(String sessionKey, ListMachines listMachine, ListMachineOptions options) throws InternalUMSException {
    return VISHNUJNI.listMachines(sessionKey, ListMachines.getCPtr(listMachine), listMachine, ListMachineOptions.getCPtr(options), options);
  }

  public static int listHistoryCmd(String sessionKey, ListCommands listCommands, ListCmdOptions options) throws InternalUMSException {
    return VISHNUJNI.listHistoryCmd(sessionKey, ListCommands.getCPtr(listCommands), listCommands, ListCmdOptions.getCPtr(options), options);
  }

  public static int listOptions(String sessionKey, ListOptionsValues listOptValues, ListOptOptions options) throws InternalUMSException {
    return VISHNUJNI.listOptions(sessionKey, ListOptionsValues.getCPtr(listOptValues), listOptValues, ListOptOptions.getCPtr(options), options);
  }

  public static int listUsers(String sessionKey, ListUsers listUsers, ListUsersOptions listOptions) throws InternalUMSException {
    return VISHNUJNI.listUsers(sessionKey, ListUsers.getCPtr(listUsers), listUsers, ListUsersOptions.getCPtr(listOptions), listOptions);
  }

  public static int addAuthSystem(String sessionKey, AuthSystem newAuthSys) throws InternalUMSException {
    return VISHNUJNI.addAuthSystem(sessionKey, AuthSystem.getCPtr(newAuthSys), newAuthSys);
  }

  public static int updateAuthSystem(String sessionKey, AuthSystem AuthSys) throws InternalUMSException {
    return VISHNUJNI.updateAuthSystem(sessionKey, AuthSystem.getCPtr(AuthSys), AuthSys);
  }

  public static int deleteAuthSystem(String sessionKey, String authSystemId) throws InternalUMSException {
    return VISHNUJNI.deleteAuthSystem(sessionKey, authSystemId);
  }

  public static int listAuthSystems(String sessionKey, ListAuthSystems listAuthSys, ListAuthSysOptions options) throws InternalUMSException {
    return VISHNUJNI.listAuthSystems(sessionKey, ListAuthSystems.getCPtr(listAuthSys), listAuthSys, ListAuthSysOptions.getCPtr(options), options);
  }

  public static int addAuthAccount(String sessionKey, AuthAccount authAccount) throws InternalUMSException {
    return VISHNUJNI.addAuthAccount(sessionKey, AuthAccount.getCPtr(authAccount), authAccount);
  }

  public static int updateAuthAccount(String sessionKey, AuthAccount authenAccount) throws InternalUMSException {
    return VISHNUJNI.updateAuthAccount(sessionKey, AuthAccount.getCPtr(authenAccount), authenAccount);
  }

  public static int deleteAuthAccount(String sessionKey, String authSystemId, String userIdOption) throws InternalUMSException {
    return VISHNUJNI.deleteAuthAccount(sessionKey, authSystemId, userIdOption);
  }

  public static int listAuthAccounts(String sessionKey, ListAuthAccounts listAuthAccounts, ListAuthAccOptions options) throws InternalUMSException {
    return VISHNUJNI.listAuthAccounts(sessionKey, ListAuthAccounts.getCPtr(listAuthAccounts), listAuthAccounts, ListAuthAccOptions.getCPtr(options), options);
  }

  public static int vishnuInitialize(String cfg) throws InternalUMSException {
    return VISHNUJNI.vishnuInitialize(cfg);
  }

  public static void vishnuFinalize() throws InternalUMSException {
    VISHNUJNI.vishnuFinalize();
  }

  public static int submitJob(String sessionKey, String machineId, String scriptFilePath, Job jobInfo, SubmitOptions options) throws InternalTMSException {
    return VISHNUJNI.submitJob(sessionKey, machineId, scriptFilePath, Job.getCPtr(jobInfo), jobInfo, SubmitOptions.getCPtr(options), options);
  }

  public static int addWork(String sessionKey, SWIGTYPE_p_TMS_Data__Work newWork) throws InternalTMSException {
    return VISHNUJNI.addWork(sessionKey, SWIGTYPE_p_TMS_Data__Work.getCPtr(newWork));
  }

  public static int cancelJob(String sessionKey, String machineId, String jobId) throws InternalTMSException {
    return VISHNUJNI.cancelJob(sessionKey, machineId, jobId);
  }

  public static int getJobInfo(String sessionKey, String machineId, String jobId, Job jobInfos) throws InternalTMSException {
    return VISHNUJNI.getJobInfo(sessionKey, machineId, jobId, Job.getCPtr(jobInfos), jobInfos);
  }

  public static int listJobs(String sessionKey, String machineId, ListJobs listOfJobs, ListJobsOptions options) throws InternalTMSException {
    return VISHNUJNI.listJobs(sessionKey, machineId, ListJobs.getCPtr(listOfJobs), listOfJobs, ListJobsOptions.getCPtr(options), options);
  }

  public static int getJobProgress(String sessionKey, String machineId, ListProgression listOfProgress, ProgressOptions options) throws InternalTMSException {
    return VISHNUJNI.getJobProgress(sessionKey, machineId, ListProgression.getCPtr(listOfProgress), listOfProgress, ProgressOptions.getCPtr(options), options);
  }

  public static int listQueues(String sessionKey, String machineId, ListQueues listofQueues, String queueName) throws InternalTMSException {
    return VISHNUJNI.listQueues(sessionKey, machineId, ListQueues.getCPtr(listofQueues), listofQueues, queueName);
  }

  public static int getJobOutput(String sessionKey, String machineId, String jobId, JobResult outputInfo, String outDir) throws InternalTMSException {
    return VISHNUJNI.getJobOutput(sessionKey, machineId, jobId, JobResult.getCPtr(outputInfo), outputInfo, outDir);
  }

  public static int getCompletedJobsOutput(String sessionKey, String machineId, ListJobResults listOfResults, String outDir) throws InternalTMSException {
    return VISHNUJNI.getCompletedJobsOutput(sessionKey, machineId, ListJobResults.getCPtr(listOfResults), listOfResults, outDir);
  }

  public static int exportCommands(String sessionKey, String oldSessionId, String filename, ExportOp op) throws InternalIMSException {
    return VISHNUJNI.exportCommands(sessionKey, oldSessionId, filename, ExportOp.getCPtr(op), op);
  }

  public static int getMetricCurrentValue(String sessionKey, String machineId, ListMetric val, CurMetricOp op) throws InternalIMSException {
    return VISHNUJNI.getMetricCurrentValue(sessionKey, machineId, ListMetric.getCPtr(val), val, CurMetricOp.getCPtr(op), op);
  }

  public static int getMetricHistory(String sessionKey, String machineId, ListMetric list, MetricHistOp op) throws InternalIMSException {
    return VISHNUJNI.getMetricHistory(sessionKey, machineId, ListMetric.getCPtr(list), list, MetricHistOp.getCPtr(op), op);
  }

  public static int getProcesses(String sessionKey, ListProcesses list, ProcessOp op) throws InternalIMSException {
    return VISHNUJNI.getProcesses(sessionKey, ListProcesses.getCPtr(list), list, ProcessOp.getCPtr(op), op);
  }

  public static int setSystemInfo(String sessionKey, SystemInfo sys) throws InternalIMSException {
    return VISHNUJNI.setSystemInfo(sessionKey, SystemInfo.getCPtr(sys), sys);
  }

  public static int setSystemThreshold(String sessionKey, Threshold threshold) throws InternalIMSException {
    return VISHNUJNI.setSystemThreshold(sessionKey, Threshold.getCPtr(threshold), threshold);
  }

  public static int getSystemThreshold(String sessionKey, ListThreshold list, ThresholdOp op) throws InternalIMSException {
    return VISHNUJNI.getSystemThreshold(sessionKey, ListThreshold.getCPtr(list), list, ThresholdOp.getCPtr(op), op);
  }

  public static int defineUserIdentifier(String sessionKey, String fmt) throws InternalIMSException {
    return VISHNUJNI.defineUserIdentifier(sessionKey, fmt);
  }

  public static int defineMachineIdentifier(String sessionKey, String fmt) throws InternalIMSException {
    return VISHNUJNI.defineMachineIdentifier(sessionKey, fmt);
  }

  public static int defineJobIdentifier(String sessionKey, String fmt) throws InternalIMSException {
    return VISHNUJNI.defineJobIdentifier(sessionKey, fmt);
  }

  public static int defineTransferIdentifier(String sessionKey, String fmt) throws InternalIMSException {
    return VISHNUJNI.defineTransferIdentifier(sessionKey, fmt);
  }

  public static int defineAuthIdentifier(String sessionKey, String fmt) throws InternalIMSException {
    return VISHNUJNI.defineAuthIdentifier(sessionKey, fmt);
  }

  public static int defineWorkIdentifier(String sessionKey, String fmt) throws InternalIMSException {
    return VISHNUJNI.defineWorkIdentifier(sessionKey, fmt);
  }

  public static int loadShed(String sessionKey, String machineId, int loadShedType) throws InternalIMSException {
    return VISHNUJNI.loadShed(sessionKey, machineId, loadShedType);
  }

  public static int setUpdateFrequency(String sessionKey, int freq) throws InternalIMSException {
    return VISHNUJNI.setUpdateFrequency(sessionKey, freq);
  }

  public static int getUpdateFrequency(String sessionKey, int[] freq) throws InternalIMSException {
    return VISHNUJNI.getUpdateFrequency(sessionKey, freq);
  }

  public static int stop(String sessionKey, Process proc) throws InternalIMSException {
    return VISHNUJNI.stop(sessionKey, Process.getCPtr(proc), proc);
  }

  public static int getSystemInfo(String sessionKey, ListSysInfo list, SysInfoOp op) throws InternalIMSException {
    return VISHNUJNI.getSystemInfo(sessionKey, ListSysInfo.getCPtr(list), list, SysInfoOp.getCPtr(op), op);
  }

  public static int restart(String sessionKey, String machineId, RestartOp op) throws InternalIMSException {
    return VISHNUJNI.restart(sessionKey, machineId, RestartOp.getCPtr(op), op);
  }

  public static int createFile(String sessionKey, String path) throws InternalFMSException {
    return VISHNUJNI.createFile(sessionKey, path);
  }

  public static int chGrp(String sessionKey, String group, String path) throws InternalFMSException {
    return VISHNUJNI.chGrp(sessionKey, group, path);
  }

  public static int chMod(String sessionKey, long mode, String path) throws InternalFMSException {
    return VISHNUJNI.chMod(sessionKey, mode, path);
  }

  public static int copyFile(String sessionKey, String src, String dest, CpFileOptions options) throws InternalFMSException {
    return VISHNUJNI.copyFile(sessionKey, src, dest, CpFileOptions.getCPtr(options), options);
  }

  public static int copyAsyncFile(String sessionKey, String src, String dest, FileTransfer transferInfo, CpFileOptions options) throws InternalFMSException {
    return VISHNUJNI.copyAsyncFile(sessionKey, src, dest, FileTransfer.getCPtr(transferInfo), transferInfo, CpFileOptions.getCPtr(options), options);
  }

  public static int headOfFile(String sessionKey, String path, java.lang.String[] contentOfFile, HeadOfFileOptions options) throws InternalFMSException {
    return VISHNUJNI.headOfFile(sessionKey, path, contentOfFile, HeadOfFileOptions.getCPtr(options), options);
  }

  public static int contentOfFile(String sessionKey, String path, java.lang.String[] contentOfFile) throws InternalFMSException {
    return VISHNUJNI.contentOfFile(sessionKey, path, contentOfFile);
  }

  public static int listDir(String sessionKey, String path, DirEntryList dirContent, LsDirOptions options) throws InternalFMSException {
    return VISHNUJNI.listDir(sessionKey, path, DirEntryList.getCPtr(dirContent), dirContent, LsDirOptions.getCPtr(options), options);
  }

  public static int createDir(String sessionKey, String path, CreateDirOptions options) throws InternalFMSException {
    return VISHNUJNI.createDir(sessionKey, path, CreateDirOptions.getCPtr(options), options);
  }

  public static int moveFile(String sessionKey, String src, String dest, CpFileOptions options) throws InternalFMSException {
    return VISHNUJNI.moveFile(sessionKey, src, dest, CpFileOptions.getCPtr(options), options);
  }

  public static int moveAsyncFile(String sessionKey, String src, String dest, FileTransfer transferInfo, CpFileOptions options) throws InternalFMSException {
    return VISHNUJNI.moveAsyncFile(sessionKey, src, dest, FileTransfer.getCPtr(transferInfo), transferInfo, CpFileOptions.getCPtr(options), options);
  }

  public static int removeFile(String sessionKey, String path, RmFileOptions options) throws InternalFMSException {
    return VISHNUJNI.removeFile(sessionKey, path, RmFileOptions.getCPtr(options), options);
  }

  public static int removeDir(String sessionKey, String path) throws InternalFMSException {
    return VISHNUJNI.removeDir(sessionKey, path);
  }

  public static int tailOfFile(String sessionKey, String path, java.lang.String[] contentOfFile, TailOfFileOptions options) throws InternalFMSException {
    return VISHNUJNI.tailOfFile(sessionKey, path, contentOfFile, TailOfFileOptions.getCPtr(options), options);
  }

  public static int getFileInfo(String sessionKey, String path, FileStat filesInfo) throws InternalFMSException {
    return VISHNUJNI.getFileInfo(sessionKey, path, FileStat.getCPtr(filesInfo), filesInfo);
  }

  public static int stopFileTransfer(String sessionKey, StopTransferOptions options) throws InternalFMSException {
    return VISHNUJNI.stopFileTransfer(sessionKey, StopTransferOptions.getCPtr(options), options);
  }

  public static int listFileTransfers(String sessionKey, FileTransferList fileTransferList, LsTransferOptions options) throws InternalFMSException {
    return VISHNUJNI.listFileTransfers(sessionKey, FileTransferList.getCPtr(fileTransferList), fileTransferList, LsTransferOptions.getCPtr(options), options);
  }

}
