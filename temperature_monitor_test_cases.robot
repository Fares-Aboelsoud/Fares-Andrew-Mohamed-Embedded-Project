*** Settings ***
Documentation     KH5048CEM Element 2 - Human Machine Interface Test Cases
...               Validates temperature limits (LL/LH), ADC conversion, LCD status (OK/NOK),
...               and LED/Buzzer alarm logic for the ATmega328P temperature monitor.

*** Variables ***
${LL}         20
${LH}         38
${TEMP}       25
${STATUS}     OK
${ALERT}      0

*** Keywords ***
Power On System
    Set Suite Variable    ${LL}        20
    Set Suite Variable    ${LH}        38
    Set Suite Variable    ${TEMP}      25
    Evaluate Alarm Status

Evaluate Alarm Status
    IF    ${TEMP} < ${LL} or ${TEMP} > ${LH}
        Set Suite Variable    ${STATUS}    NOK
        Set Suite Variable    ${ALERT}     1
    ELSE
        Set Suite Variable    ${STATUS}    OK
        Set Suite Variable    ${ALERT}     0
    END

Press Button
    [Arguments]    ${btn}
    IF    '${btn}' == 'UP' and ${LH} < 99
        ${new_lh}=    Evaluate    ${LH} + 1
        Set Suite Variable    ${LH}    ${new_lh}
    ELSE IF    '${btn}' == 'DOWN' and ${LH} > (${LL} + 1)
        ${new_lh}=    Evaluate    ${LH} - 1
        Set Suite Variable    ${LH}    ${new_lh}
    ELSE IF    '${btn}' == 'RIGHT' and ${LL} < (${LH} - 1)
        ${new_ll}=    Evaluate    ${LL} + 1
        Set Suite Variable    ${LL}    ${new_ll}
    ELSE IF    '${btn}' == 'LEFT' and ${LL} > 0
        ${new_ll}=    Evaluate    ${LL} - 1
        Set Suite Variable    ${LL}    ${new_ll}
    END
    Evaluate Alarm Status

Apply Sensor Voltage mV
    [Arguments]    ${voltage_mv}
    ${adc}=     Evaluate    round(${voltage_mv} * 1024 / 5000)
    ${temp}=    Evaluate    int((${adc} * 500) / 1024)
    Set Suite Variable    ${TEMP}    ${temp}
    Evaluate Alarm Status

*** Test Cases ***
T1: Power On Default State
    [Documentation]    Verify default limits LL:20, LH:38, live temperature, and OK status on boot.
    # Arrange & Act
    Power On System

    # Assert
    Should Be Equal As Integers    ${LL}        20
    Should Be Equal As Integers    ${LH}        38
    Should Be Equal As Strings     ${STATUS}    OK
    Should Be Equal As Integers    ${ALERT}     0

T2: Tap Each Arrow Button Once
    [Documentation]    Verify UP/DOWN change LH by 1 and RIGHT/LEFT change LL by 1 independently.
    # Arrange
    Power On System

    # Act & Assert
    Press Button    UP
    Should Be Equal As Integers    ${LH}    39
    Should Be Equal As Integers    ${LL}    20

    Press Button    DOWN
    Should Be Equal As Integers    ${LH}    38
    Should Be Equal As Integers    ${LL}    20

    Press Button    RIGHT
    Should Be Equal As Integers    ${LL}    21
    Should Be Equal As Integers    ${LH}    38

    Press Button    LEFT
    Should Be Equal As Integers    ${LL}    20
    Should Be Equal As Integers    ${LH}    38

T3: Room Temperature Accuracy Within 1 Degree
    [Documentation]    Verify LM35 reading matches reference thermometer within +-1 C.
    # Arrange
    Power On System
    ${ref_temp}=    Set Variable    25

    # Act (250 mV corresponds to 25 C on LM35)
    Apply Sensor Voltage mV    250
    ${diff}=    Evaluate    abs(${TEMP} - ${ref_temp})

    # Assert
    Should Be True    ${diff} <= 1

T4: Upper Boundary Limit On LH
    [Documentation]    Verify LH stops at 99 when pressing UP 3 times starting from 98.
    # Arrange
    Power On System
    Set Suite Variable    ${LH}    98

    # Act
    Press Button    UP
    Press Button    UP
    Press Button    UP

    # Assert
    Should Be Equal As Integers    ${LH}    99

T5: Lower Boundary Limit On LL
    [Documentation]    Verify LL stops at 0 when pressing LEFT 3 times starting from 1.
    # Arrange
    Power On System
    Set Suite Variable    ${LL}    1

    # Act
    Press Button    LEFT
    Press Button    LEFT
    Press Button    LEFT

    # Assert
    Should Be Equal As Integers    ${LL}    0

T6: Prevent LL From Crossing LH
    [Documentation]    Verify LL stops at 37 when LH is 38 and RIGHT is pressed 3 times from 36.
    # Arrange
    Power On System
    Set Suite Variable    ${LL}    36
    Set Suite Variable    ${LH}    38

    # Act
    Press Button    RIGHT
    Press Button    RIGHT
    Press Button    RIGHT

    # Assert
    Should Be Equal As Integers    ${LL}    37

T7: Temperature Below Low Limit Triggers Alarm
    [Documentation]    Verify 195 mV (19 C) triggers NOK status and activates LED and buzzer.
    # Arrange
    Power On System

    # Act
    Apply Sensor Voltage mV    195

    # Assert
    Should Be Equal As Integers    ${TEMP}      19
    Should Be Equal As Strings     ${STATUS}    NOK
    Should Be Equal As Integers    ${ALERT}     1

T8: Exact Boundary Limits Remain OK
    [Documentation]    Verify 200 mV (20 C) and 386 mV (38 C) both stay OK with alarm off.
    # Arrange
    Power On System

    # Act & Assert (Lower limit = 20 C)
    Apply Sensor Voltage mV    200
    Should Be Equal As Integers    ${TEMP}      20
    Should Be Equal As Strings     ${STATUS}    OK
    Should Be Equal As Integers    ${ALERT}     0

    # Act & Assert (Upper limit = 38 C)
    Apply Sensor Voltage mV    386
    Should Be Equal As Integers    ${TEMP}      38
    Should Be Equal As Strings     ${STATUS}    OK
    Should Be Equal As Integers    ${ALERT}     0

T9: Temperature Above High Limit Triggers Alarm
    [Documentation]    Verify 391 mV (39 C) triggers NOK status and activates LED and buzzer.
    # Arrange
    Power On System

    # Act
    Apply Sensor Voltage mV    391

    # Assert
    Should Be Equal As Integers    ${TEMP}      39
    Should Be Equal As Strings     ${STATUS}    NOK
    Should Be Equal As Integers    ${ALERT}     1

T10: Sensor Heating And Cooling Recovery
    [Documentation]    Verify warming LM35 above LH triggers alarm and cooling back clears it.
    # Arrange
    Power On System

    # Act (Warm above 38 C -> 40 C)
    Apply Sensor Voltage mV    400
    Should Be Equal As Strings     ${STATUS}    NOK
    Should Be Equal As Integers    ${ALERT}     1

    # Act (Cool back into valid range -> 28 C)
    Apply Sensor Voltage mV    280

    # Assert
    Should Be Equal As Strings     ${STATUS}    OK
    Should Be Equal As Integers    ${ALERT}     0