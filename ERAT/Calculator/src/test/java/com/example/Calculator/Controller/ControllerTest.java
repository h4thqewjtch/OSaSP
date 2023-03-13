package com.example.Calculator.Controller;

import com.example.Calculator.CacheService.CacheService;
import com.example.Calculator.Calculator.Calculator;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.http.HttpStatus;

import static org.junit.jupiter.api.Assertions.*;

class ControllerTest {

    Controller result;
    Calculator calculator;
    CacheService cacheService;

    @BeforeEach
    void Init() {
        calculator = new Calculator();
        cacheService = new CacheService(calculator);
        result = new Controller(cacheService);
    }

    @Test
    void summationRightRequest() {
        assertEquals(HttpStatus.OK, result.summation("36", "4").getStatusCode());
    }

    @Test
    void summationWrongTypeVar1() {
        assertEquals(HttpStatus.BAD_REQUEST, result.summation("dgf", "4").getStatusCode());
    }

    @Test
    void summationWrongTypeVar2() {
        assertEquals(HttpStatus.BAD_REQUEST, result.summation("36", "e").getStatusCode());
    }

    @Test
    void subtractionRightRequest() {
        assertEquals(HttpStatus.OK, result.subtraction("4", "36").getStatusCode());
    }

    @Test
    void subtractionWrongTypeVar1() {
        assertEquals(HttpStatus.BAD_REQUEST, result.subtraction("dgf", "4").getStatusCode());
    }

    @Test
    void subtractionWrongTypeVar2() {
        assertEquals(HttpStatus.BAD_REQUEST, result.subtraction("36", "e").getStatusCode());
    }

    @Test
    void divisionRightRequest() {
        assertEquals(HttpStatus.OK, result.division("36", "4").getStatusCode());
    }

    @Test
    void divisionWrongTypeVar1() {
        assertEquals(HttpStatus.BAD_REQUEST, result.division("dgf", "4").getStatusCode());
    }

    @Test
    void divisionWrongTypeVar2() {
        assertEquals(HttpStatus.BAD_REQUEST, result.division("36", "e").getStatusCode());
    }

    @Test
    void divisionWrongValueVar2() {
        assertEquals(HttpStatus.BAD_REQUEST, result.division("36", "0").getStatusCode());
    }

    @Test
    void multiplicationRightRequest() {
        assertEquals(HttpStatus.OK, result.multiplication("36", "4").getStatusCode());
    }

    @Test
    void multiplicationWrongTypeVar1() {
        assertEquals(HttpStatus.BAD_REQUEST, result.multiplication("dgf", "4").getStatusCode());
    }

    @Test
    void multiplicationWrongTypeVar2() {
        assertEquals(HttpStatus.BAD_REQUEST, result.multiplication("36", "e").getStatusCode());
    }
}