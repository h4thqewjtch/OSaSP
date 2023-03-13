package com.example.Calculator.Checking;


public class Checking extends Exception {

    public static String CheckType(String var1, String var2) {
        try {
            Double.parseDouble(var1);
        } catch (Exception e) {
            //return "error: Type of parameter <<var1>> is wrong!";
        }
        try {
            Double.parseDouble(var2);
        } catch (Exception e) {
            //return "error: Type of parameter <<var2>> is wrong!";
        }
        return "result: ";
    }

    public static boolean CheckValue(double var) {
        return var != 0;
    }
}

