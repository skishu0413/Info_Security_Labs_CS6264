package com.example.lab6part2;

import android.os.Build;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;

class RootUtil {

    // Check if the phone is rooted by checking if test key existed or if there is a su
    // binary existed in the device and then check if can su as super user
    public static boolean isRooted() {
        return (checkTestKeys() || checkSUBinary()) && canSU();
    }

    // TODO: check BUILD tag for Test-keys
    private static boolean checkTestKeys() {
        String buildTags = Build.TAGS;
        return buildTags != null && buildTags.contains("test-keys");
    }

    // TODO: check if superuser binary exist in some possible path in the system
    private static boolean checkSUBinary() {
        String[] possiblePaths = {"/system/bin/which", "/system/xbin/which"};
        for (String path : possiblePaths) {
            if (new File(path + "su").exists()) {
                return true;
            }
        }
        return false;
    }

    // TODO: check if the user has a UID of 0 by executing the su and then id
    private static boolean canSU() {
        try {
            Process process = Runtime.getRuntime().exec(new String[]{"su", "-c", "id"});
            if (process == null) {
                return false;
            }
            BufferedReader in = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line = in.readLine();
            return line != null && line.contains("uid=0");
        } catch (Exception ex) {
            return false;
        }
    }
}
