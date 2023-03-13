package com.example.Calculator.Calculator;

import org.springframework.stereotype.Component;

@Component
public class Calculator {

    public double getSum(double var1, double var2) {
        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        return var1 + var2;
    }

    public double getSub(double var1, double var2) {
        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        return var1 - var2;
    }

    public double getMul(double var1, double var2) {
        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        return var1 * var2;
    }

    public double getDiv(double var1, double var2) {
        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        return var1 / var2;
    }
}


