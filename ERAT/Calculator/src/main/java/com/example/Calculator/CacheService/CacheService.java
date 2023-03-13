package com.example.Calculator.CacheService;

import com.example.Calculator.Calculator.Calculator;
import org.springframework.cache.interceptor.SimpleKey;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.Map;

@Service
public class CacheService {
    private final Calculator calculator;
    private final Map<SimpleKey, Double> cacheSum;

    public CacheService(Calculator calculator) {
        this.calculator = calculator;
        cacheSum = new HashMap<SimpleKey, Double>();
    }

    public Double getValue(double var1, double var2) {
        return cacheSum.get([var1,var2]);
    }

    public String evict() {
        if(cacheSum.size()==0)
            return "Cache entries are not exist!";
        cacheSum.clear();
        return "Current Status : All cache entries have been evicted!";
    };

    public double getSum(double var1, double var2) {
        if(cacheSum.get(SimpleKey[var1,var2])==null) {
            cacheSum.put("Sum", calculator.getSum(var1, var2));
            return calculator.getSum(var1, var2);
        }
        return cacheManager.get("Sum");
    }

    public double getSub(double var1, double var2) {
        if(cacheManager.get("Sub")==null) {
            cacheManager.put("Sub", calculator.getSub(var1, var2));
            return calculator.getSub(var1, var2);
        }
        return cacheManager.get("Sub");
    }
    public double getDiv(double var1, double var2) {
        if(cacheManager.get("Div")==null) {
            cacheManager.put("Div", calculator.getDiv(var1, var2));
            return calculator.getDiv(var1, var2);
        }
        return cacheManager.get("Div");
    }

    public double getMul(double var1, double var2) {
        if(cacheManager.get("Mul")==null) {
            cacheManager.put("Mul", calculator.getMul(var1, var2));
            return calculator.getSum(var1, var2);
        }
        return cacheManager.get("Mul");
    }
}