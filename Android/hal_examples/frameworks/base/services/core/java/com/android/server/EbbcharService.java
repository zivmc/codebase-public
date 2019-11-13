package com.android.server;

import android.content.Context;
import android.os.IEbbcharService;
import android.util.Slog;

public class EbbcharService extends IEbbcharService.Stub {
	private static final String TAG = "EbbcharService";

	private long mPtr = 0;

	EbbcharService() {
		mPtr = init_native();

		if(mPtr == 0) {
			Slog.e(TAG, "Failed to initialize ebbchar service.");
		}
	}

	public void setInput(String input) {
		if(mPtr == 0) {
			Slog.e(TAG, "Ebbchar service is not initialized.");
			return;
		}

		setInput_native(mPtr, input);
	}

	public String getOutput() {
		if(mPtr == 0) {
			Slog.e(TAG, "Ebbchar service is not initialized.");
			return "";
		}

		return getOutput_native(mPtr);
	}

	private static native long init_native();
	private static native void setInput_native(long ptr, String input);
	private static native String getOutput_native(long ptr);
};
