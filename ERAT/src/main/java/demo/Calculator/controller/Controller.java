package demo.Calculator.controller;

import demo.Calculator.checking.Checking;
import demo.Calculator.initializator.Initializator;
import demo.Calculator.model.Calculator;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("calc")
public class Controller {
    @GetMapping
    public ResponseEntity<String> multi() {
        return new ResponseEntity<>("error: type of operation(sum/sub/div/mul) does not exist!", HttpStatus.BAD_REQUEST);
    }
    @GetMapping("sum")
    public ResponseEntity<String> summation(@RequestParam("var1") String var1,
                                            @RequestParam("var2") String var2) {
        Initializator sum = new Initializator(var1, var2);
        String str = sum.initialise();
        if (!str.equals("result: "))
            return new ResponseEntity<>(str, HttpStatus.BAD_REQUEST);
        Calculator result = new Calculator(Double.parseDouble(var1), Double.parseDouble(var2));
        return new ResponseEntity<>(str + result.getSum(), HttpStatus.OK);
    }

    @GetMapping("sub")
    public ResponseEntity<String> subtraction(@RequestParam("var1") String var1,
                                              @RequestParam("var2") String var2) {
        Initializator sub = new Initializator(var1, var2);
        String str = sub.initialise();
        if (!str.equals("result: "))
            return new ResponseEntity<>(str, HttpStatus.BAD_REQUEST);
        Calculator result = new Calculator(Double.parseDouble(var1), Double.parseDouble(var2));
        return new ResponseEntity<>(str + result.getSub(), HttpStatus.OK);
    }

    @GetMapping("div")
    public ResponseEntity<String> division(@RequestParam("var1") String var1,
                                           @RequestParam("var2") String var2) {
        Initializator div = new Initializator(var1, var2);
        String str = div.initialise();
        if (!str.equals("result: "))
            return new ResponseEntity<>(str, HttpStatus.BAD_REQUEST);
        Checking value = new Checking();
        if (!value.checkValue(Double.parseDouble(var2))) {
            return new ResponseEntity<>("error: Value of parameter <<var2>> is wrong!", HttpStatus.BAD_REQUEST);
        }
        Calculator result = new Calculator(Double.parseDouble(var1), Double.parseDouble(var2));
        return new ResponseEntity<>(str + result.getDiv(), HttpStatus.OK);
    }

    @GetMapping("mul")
    public ResponseEntity<String> multiplication(@RequestParam("var1") String var1,
                                                 @RequestParam("var2") String var2) {
        Initializator mul = new Initializator(var1, var2);
        String str = mul.initialise();
        if (!str.equals("result: "))
            return new ResponseEntity<>(str, HttpStatus.BAD_REQUEST);
        Calculator result = new Calculator(Double.parseDouble(var1), Double.parseDouble(var2));
        return new ResponseEntity<>(str + result.getMul(), HttpStatus.OK);
    }
}