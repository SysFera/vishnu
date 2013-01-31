package com.sysfera.vishnu.api;

import com.sysfera.vishnu.api.vishnu.internal.ConnectOptions;
import com.sysfera.vishnu.api.vishnu.internal.InternalUMSException;
import com.sysfera.vishnu.api.vishnu.internal.Session;
import com.sysfera.vishnu.api.vishnu.internal.VISHNU;

public class LoadLibrary {
	// DIET value for already diet_init done
	static final int GRPC_ALREADY_INITIALIZED = 15;

	public static void LoadVishnuLibrary() {
		System.loadLibrary("VISHNU");
	}

	// To init the UMS module
	private static void initUMS() {
		int res = 0;
		String VISHNU_CONFIG_FILE = System.getenv("VISHNU_CONFIG_FILE");
	//	String VISHNU_CONFIG_FILE = "/home/phi/Soft/vishnu/conf/diet/client.cfg";
		try {
			res = VISHNU.vishnuInitialize(VISHNU_CONFIG_FILE);
		} catch (InternalUMSException e) {

		}
		if ((res != 0) && (res != GRPC_ALREADY_INITIALIZED)) {
			throw new RuntimeException("Cannot initialize VISHNU Java Library");
		}
	}

	public static void main(String[] args) {
		LoadLibrary.LoadVishnuLibrary();
		Session currentSession = new Session();

		try {
			initUMS();
			ConnectOptions op = new ConnectOptions();
			VISHNU.connect("root", "vishnu_user", currentSession, op);

		} catch (InternalUMSException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
}
