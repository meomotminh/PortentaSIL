## Design 

- mainly all simulators have the same test setup topology in common, which is based on 2 different components. A standard *workstation* generates the static and dynamic config and is used for the analysis of the performed test case + the *residual bus simulator platform* execute the simulation model in real-time and generates emulated network packages.  

- interconnect event-based simulation platform OMNet++ with RT Ethernet :
    - a microcontroller executes a RT Ethernet stack -> responsible for protocol conform reception and transmission of messages from and to the real network
    - simulation model of the same RT Ethernet protocol runs on a standard workstation inside OMNeT simulation envronment
        - both connected via dual-port memory to exchange data in both direction
- to send and recevie time-triggered messages from the simulation to the real world and vice versa, the simulation environment was synch with the real network, by accessing highly precise clock of the microcontroller.

- 2 hardwares:
    - a work station run the simulation environment and generate the packages to be transmitted
    - microcontroller is used to send the packages in conformance with transmission strategy
        - simulator is responsible for generating emulated network frames -> env implement a discrete time-based simulation approach, to generate emulated frames at the desired point-in-time
    - to decribe tes case for functional requirements: abstract test case -> utilize a model is independent of the actual test env so that the same test case can be executed on different test platforms.
    - their presented abstract test case model bases on the state space model, allow formally model the input and outputs adn the internal states as vectors of a SUT
    - an abstract test case define the data of the inputs to be generated and the expected outputs and the internal states of the SUT in dependency of the time.  
    - non-functional timing requirements are to be validated too, this model is extended with temporal attributes such as latency

- Challenge:
    - commercial residual bus simulators already support state of the art automotive bus network tech
    - since the utilization of RT Ethernet as in-car network system is novel, a suitable simulation env has diffrerent attributes and requirements to be fulfilled.

- Message Classes and Synchronization
    - connected via physical communicatino interface to the SUT and generate emulated data packages
    - to validate behavior of the SUT, the residual bus simulator must completely support the utilized network protovol
    - depending on the synch role of the SUT, the residual bus simulator has to perform the opposite role 
    - simulation is more complex than a residual bus simulation 

- parallel data transmission, test setup for a residual bus simulation in RT Ethernet networks differs from the previously presented approach.
    - directly and indirectly connected approaches
        - in a directly connected setup simulator communicate directly with the SUT
            - limited to 1 single SUT
            - only be utilized for unit testing
        - indirectly connected approach utilize an additional RT Ethernet switch for communication

- modelling with *MARTE* & abstract test case models
    - test cases are derived by models that represent system requirements.
    - the presented residual bus simulation approach is utilized to validate functional and non-functional timing requirements.
    - *MARTE* profile for *UML*

- description of the sample use-case
    - both time-critical (TT and RC messages) as well as BE-messages are concurrently transferred
        - headlight controller must acknowledge every received valid light status by replying the status
        - headlight controller must periodically announce its current light status to the netework, so that further app can use the state of the headlight.
    - non-funtional:
        - acknowledgement ust arrive at a delay of 500 micros +- 50 micros
        - transmission rate of the light status announcements must arrive at an interval of 5000 micros +- 5 micros

- Modeling Non-funtional timing requirements with MARTE
    - UML profile MARTE is utilized to model and analyze real-time and embedded systems and builds on the wide range of supported UML models
    - profile extend UML with Value Specific Language (VSL) -> annotate classic UML diagrams and allow for describing different scale units, as well as mathematical expression
    - detailed clock model -> ideal synchronized clock is appropriate

- Message Processing
    - TimeConstrained
        on = MARTE_Library::TimedLibrary::idealClock
        (messageReceived - replySended) = 500
        jitter(messageReceived - replySended) <= 100
    -------------------------------------------------------------------------- 
    Dashboard-System -> setNewLightStatus(status)      -> Headlight-Controller
                     <- sendCurrentLightStatus(status) <-

    - TimeStateTransmission
        on = MARTE_Library::TimedLibrary::idealClock
        on = MARTE_Library::TimedLibrary::idealClock
        (sendStatus(i) - sendStatus(i-1)) = 5000
        jitter(sendStatus(i) - sendStatus(i-1)) <= 10
    
    - Functional and non-functional timing requirements for the sample headlight control app were modeled using MARTE as an extension to standard UML

    - associated test cases for RT Ethernet residual bus simulation can be effectively derived from the developed sequence diagrams

    - state space model