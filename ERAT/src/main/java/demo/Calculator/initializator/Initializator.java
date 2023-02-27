package demo.Calculator.initializator;

import demo.Calculator.checking.Checking;

public class Initializator {
    private final String var1;
    private final String var2;

    public Initializator(String _var1, String _var2) {
        var1 = _var1;
        var2 = _var2;
    }

    public String initialise() {
        Checking result = new Checking();
        if (!result.checkType(var1)) {
            return "error: Type of parameter <<var1>> is wrong!";
        }
        if (!result.checkType(var2)) {
            return "error: Type of parameter <<var2>> is wrong!";
        }
        return "result: ";
    }
}
