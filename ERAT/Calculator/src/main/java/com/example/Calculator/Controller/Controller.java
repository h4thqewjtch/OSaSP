package com.example.Calculator.Controller;

import com.example.Calculator.CacheService.CacheService;
import com.example.Calculator.Checking.Checking;

import org.apache.juli.logging.Log;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.logging.*;

@RestController
@RequestMapping("calc")
public class Controller {
    private final CacheService cacheService;

    public Controller(CacheService cacheService) {
        this.cacheService = cacheService;
    }

    @GetMapping("evict")
    public String evict() {
        return cacheService.evict();
    }

    @GetMapping("manual")
    public String getManualData(@RequestParam("key") String key) {
        return "[" + key + "]" + ":" + "[" + cacheService.getValue(key) + "]";
    }

    @GetMapping
    public ResponseEntity<String> nul() {
        Logger LOG_NUL = Logger.getLogger(Log.class.getName());

        LOG_NUL.log(Level.WARNING, "Request Status: INTERNAL SERVER ERROR");
        return new ResponseEntity<>("error: type of operation(sum/sub/div/mul) does not exist!", HttpStatus.INTERNAL_SERVER_ERROR);
    }

    @GetMapping("sum")
    public ResponseEntity<String> summation(@RequestParam("var1") String var1,
                                            @RequestParam("var2") String var2) {
        Logger LOG_SUM = Logger.getLogger(Log.class.getName());

        String str = Checking.CheckType(var1, var2);
        if (!str.equals("result: ")) {
            LOG_SUM.log(Level.WARNING, "Request Status: BAD REQUEST");
            return new ResponseEntity<>(str, HttpStatus.BAD_REQUEST);
        }
        LOG_SUM.log(Level.INFO, "Request was successfully processed!");
        return new ResponseEntity<>(str + cacheService.getSum(Double.parseDouble(var1), Double.parseDouble(var2)), HttpStatus.OK);
    }

    @GetMapping("sub")
    public ResponseEntity<String> subtraction(@RequestParam("var1") String var1,
                                              @RequestParam("var2") String var2) {
        Logger LOG_SUB = Logger.getLogger(Log.class.getName());

        String str = Checking.CheckType(var1, var2);
        if (!str.equals("result: ")) {
            LOG_SUB.log(Level.WARNING, "Request Status: BAD REQUEST");
            return new ResponseEntity<>(str, HttpStatus.BAD_REQUEST);
        }
        LOG_SUB.log(Level.INFO, "Request was successfully processed!");
        return new ResponseEntity<>(str + cacheService.getSub(Double.parseDouble(var1), Double.parseDouble(var2)), HttpStatus.OK);
    }

    @GetMapping("div")
    public ResponseEntity<String> division(@RequestParam("var1") String var1,
                                           @RequestParam("var2") String var2) {
        Logger LOG_DIV = Logger.getLogger(Log.class.getName());

        String str = Checking.CheckType(var1, var2);
        if (!str.equals("result: ")) {
            LOG_DIV.log(Level.WARNING, "Request Status: BAD REQUEST");
            return new ResponseEntity<>(str, HttpStatus.BAD_REQUEST);
        }
        if (!Checking.CheckValue(Double.parseDouble(var2))) {
            LOG_DIV.log(Level.WARNING, "Request Status: BAD REQUEST");
            return new ResponseEntity<>("error: Value of parameter <<var2>> is wrong!", HttpStatus.BAD_REQUEST);
        }
        LOG_DIV.log(Level.INFO, "Request was successfully processed!");
        return new ResponseEntity<>(str + cacheService.getDiv(Double.parseDouble(var1), Double.parseDouble(var2)), HttpStatus.OK);
    }

    @GetMapping("mul")
    public ResponseEntity<String> multiplication(@RequestParam("var1") String var1,
                                                 @RequestParam("var2") String var2) {
        Logger LOG_MUL = Logger.getLogger(Log.class.getName());

        String str = Checking.CheckType(var1, var2);
        if (!str.equals("result: ")) {
            LOG_MUL.log(Level.WARNING, "Request Status: BAD REQUEST");
            return new ResponseEntity<>(str, HttpStatus.BAD_REQUEST);
        }
        LOG_MUL.log(Level.INFO, "Request was successfully processed!");
        return new ResponseEntity<>(str + cacheService.getMul(Double.parseDouble(var1), Double.parseDouble(var2)), HttpStatus.OK);
    }
}