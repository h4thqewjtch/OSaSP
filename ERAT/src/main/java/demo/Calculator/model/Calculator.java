package demo.Calculator.model;

public class Calculator {
    private double var1=0;
    private double var2=0;
    public Calculator(double _var1, double _var2){
        var1=_var1;
        var2=_var2;
    }
    public double getSum(){
            return var1 + var2;
    }
    public double getSub(){
        return var1 - var2;
    }
    public double getMul(){
        return var1 * var2;
    }
    public double getDiv(){
        return var1 / var2;
    }
}
