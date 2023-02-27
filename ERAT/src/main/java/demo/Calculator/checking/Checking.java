package demo.Calculator.checking;

public class Checking extends Exception {
    public boolean checkType(String _var) {
        try {
            double value = Double.parseDouble(_var);
        } catch (Exception e) {
            return false;
        }
        return true;
    }

    public boolean checkValue(double _var) {
        return _var == 0 ? false : true;
    }
}
