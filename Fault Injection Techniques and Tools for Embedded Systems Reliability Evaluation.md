# Introduction

- 3 major concerns should be posed by the procedure of the computer system design:
    - specifying system dependability requirements
    - designing and implementing the computer system so as to achieve the dependability required
    - validating the system

- dependability is general description of a system characteristic but not an attribute that can be expressed using a single metric, several metrics:
    - Reliability
    - Availability
    - Safety
    - MTTF
    - Coverage
    - Fault Latency

- dependability, the property of a computer system such that reliance can justifiably be placed on the service it delivers, which is a qualitative system attribute that is quantified through the following terminologies
    - **Reliability**: conditonal probability that the system will perform correctly throughout the interval [t0,t], given that the system was performing correctly at time t0, which concern the continuity of service
    - **Availability**: probability that a system is operating correctly and is available to perform its functions at the instant time t, which concerns the system readiness for the usage
    - **Safety**: probability that a system will either perform its functions correctly or will discontinue its functions in a manner that does not disrupt the operation of other system or compromise the safety of any people associated with the system, which concerns the non-occurence of the catastrophic consequences on the environment
    - **Mean Time To Failure (MTTF)**: expected time that a system will operate before the first failure occurs, which concerns the occurrence of the first failure
    - **Coverage**: C is the conditional probability, given the existence of a fault, the system recovers, which concern the system's ability to detect/recover a fault
    - **Fault Latency**: is the time between the occurrence of the fault and the occurrence of an error resulting from the fault
    - **Maintainability**: measure of the ease with which a system can be repaired, once it has failed, which is related to the aptitude to undergo repairs and evolution
    - **Testability**: a means by which the existence and quality of certain attributes within a system are determined, which concerns the validation and evaluation process of the system
    - **Rate of Occurrence of Failures**: measures of the time of failure occurrences during a unit time interval, which is appropriate in a system which actively controls some potentially dangerous process
    - **Probability of Failure on Demand (PFD)**: probability of a system failing to respond on demand, which is suitable for a 'safety' system and only called upon to act when another system gets into a potential unsafe condition
    - **Hazard Reduction Factor (HRF)** HRF = 1/PFD

- **Fault** is a physical defect, imperfection, or flaw that occurs within some hardware or software component: short, open circuit, divided-by-zero fault
- **Error** is a deviation from accuracy or correctness and is the manifestation of a fault: wrong voltage in circuit or infinite number
- **Failure** is non-performance of some action that is due or expected

- when a fault cause an incorrect change in a machine stage, an error occur
                Fault -> Error -> Failure

- Fault Category:
    - Fault Space: is usually a multi-dimensional space whose dimensions can include the time of occurence and duration of the fault (when), type of value or form of faults (how) and the location of the fault within the system (where)
        - only fault inserted into a typical fault location has the value to be researched
    - Harware Fault: best classified by their duration - permanent, transient, or intermittent
        - permanent: irreversible component damages
        - transient: triggered by environmental conditions such as power-line fluctuation, electromagnetic interference or radiation: occur more often than permanent ones and also much harder to detect
        - intermittent: unstable hardware or varying hardware states
    - Software Fault: consequence of incorrect design, at the specification or at coding time.
        - failure of system: transient behaviors
        - categorize:
            - **Function faults**: incorrect or missing implementation that requires a design change to correct the fault
            - **Algorithm faults**: incorrect or missing implementation that can be fixed without the need of design change but a change of the algorithm
            - **Timing/Serialization faults**: missing or incorrect serialization of shared resources. Certain mechanisms must be implemented to protect this kind of fault from happening such as MUTEX
            - **Checking faults**: missing or incorrect validation of data, incorrect loop or incorrect conditional statement
            - **Assignment faults**: values assigned incorrectly or not assigned
        - Statistical Fault Coverage Estimation:
            - **Fault Tolerance Coverage** estimations obtained through fault injection experiments are estimates of the conditional probabilistic measure characterizing dependability
            - **Force Coverage** P(F = f)
            - To determine relationship bw 2 distribution parameters, a new rando variable P is introduced, which is also a function of F and P
        
        - Fault Coverage Estimation:
            - binomial random variable Xi where Xi = 1 (covered fault)
                                                   = 0 (uncovered fault)

        - 2 major categories of fault injection techniques: 
            - execution based: the system itself is deployed, and some mechanism is used to cause faults in the system and its execution is observed to determine the effects of the faults.
            - simulation based: a model of the system is developed and faults are introduced into that model. model is then simulated to find the effects of the fault on the operation of the system

# Fault Injection Environment

- Fault Injector
- Fault Library: store different fault types, fault locations, fault times, and appropriate hardware semantics or software structures
- Workload Generator: generates the workload for the target system as input
- Workload Library: Stores sample workloads for the target system
- Controller: controll the experiment
- Monitor: tracks the executions of the commands and initiates data collection whenever necessary
- Data collector: perform online data collection
- data analyzer: perform data processing and analysis

-  for each fault injection experiment, 3 possible outcomes:
    - fault could be covered
    - fault is uncovered
    - no response

- Hardware-based Fault Injection
    - **Forcing Technique**: fault is injected directly into an integrated circuit terminal, connector without any disconnection of parts. Fault injector probe forces a low or high logical level at the selected points.
    - **Insertion Technique**: special device replace part of the circuit previously removed from its support. device injects faults. connection bw 2 circuits is cut off before injecting fault

- Fault injector should have minor interference with the exercised system
- Faults should be injected at the locations internal to the ICs in the exercised system
- Fault injected into the system are representative of the actual faults that occur within the system