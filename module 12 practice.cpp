#include <iostream>
#include <memory>
#include <string>

// Forward declaration
class Context;

// Базовый калсс состояния
class State {
public:
    virtual ~State() = default;
    virtual void handle(Context& context, const std::string& action) = 0;
    virtual std::string getName() const = 0;
};

// Класс контекста, упр состояниями
class Context {
private:
    std::unique_ptr<State> state;
public:
    explicit Context(std::unique_ptr<State> initialState) : state(std::move(initialState)) {}

    void setState(std::unique_ptr<State> newState) {
        state = std::move(newState);
    }

    void request(const std::string& action) {
        state->handle(*this, action);
    }

    std::string getStateName() const {
        return state->getName();
    }
};

// Реализация конкретных состояний
class IdleState : public State {
public:
    void handle(Context& context, const std::string& action) override;
    std::string getName() const override { return "Idle"; }
};

class CarSelectedState : public State {
public:
    void handle(Context& context, const std::string& action) override;
    std::string getName() const override { return "CarSelected"; }
};

class OrderConfirmedState : public State {
public:
    void handle(Context& context, const std::string& action) override;
    std::string getName() const override { return "OrderConfirmed"; }
};

class CarArrivedState : public State {
public:
    void handle(Context& context, const std::string& action) override;
    std::string getName() const override { return "CarArrived"; }
};

class InTripState : public State {
public:
    void handle(Context& context, const std::string& action) override;
    std::string getName() const override { return "InTrip"; }
};

class TripCompletedState : public State {
public:
    void handle(Context& context, const std::string& action) override;
    std::string getName() const override { return "TripCompleted"; }
};

class TripCancelledState : public State {
public:
    void handle(Context& context, const std::string& action) override;
    std::string getName() const override { return "TripCancelled"; }
};

// Реализация методов состояний
void IdleState::handle(Context& context, const std::string& action) {
    if (action == "select_car") {
        std::cout << "Car selected. Moving to CarSelected state.\n";
        context.setState(std::make_unique<CarSelectedState>());
    } else {
        std::cout << "Invalid action in Idle state.\n";
    }
}

void CarSelectedState::handle(Context& context, const std::string& action) {
    if (action == "confirm_order") {
        std::cout << "Order confirmed. Moving to OrderConfirmed state.\n";
        context.setState(std::make_unique<OrderConfirmedState>());
    } else if (action == "cancel") {
        std::cout << "Order cancelled. Moving to TripCancelled state.\n";
        context.setState(std::make_unique<TripCancelledState>());
    } else {
        std::cout << "Invalid action in CarSelected state.\n";
    }
}

void OrderConfirmedState::handle(Context& context, const std::string& action) {
    if (action == "car_arrived") {
        std::cout << "Car arrived. Moving to CarArrived state.\n";
        context.setState(std::make_unique<CarArrivedState>());
    } else if (action == "cancel") {
        std::cout << "Order cancelled. Moving to TripCancelled state.\n";
        context.setState(std::make_unique<TripCancelledState>());
    } else {
        std::cout << "Invalid action in OrderConfirmed state.\n";
    }
}

void CarArrivedState::handle(Context& context, const std::string& action) {
    if (action == "start_trip") {
        std::cout << "Trip started. Moving to InTrip state.\n";
        context.setState(std::make_unique<InTripState>());
    } else if (action == "cancel") {
        std::cout << "Trip cancelled. Moving to TripCancelled state.\n";
        context.setState(std::make_unique<TripCancelledState>());
    } else {
        std::cout << "Invalid action in CarArrived state.\n";
    }
}

void InTripState::handle(Context& context, const std::string& action) {
    if (action == "complete_trip") {
        std::cout << "Trip completed. Moving to TripCompleted state.\n";
        context.setState(std::make_unique<TripCompletedState>());
    } else if (action == "cancel") {
        std::cout << "Trip cancelled. Moving to TripCancelled state.\n";
        context.setState(std::make_unique<TripCancelledState>());
    } else {
        std::cout << "Invalid action in InTrip state.\n";
    }
}

void TripCompletedState::handle(Context& context, const std::string&) {
    std::cout << "Trip completed. Returning to Idle state.\n";
    context.setState(std::make_unique<IdleState>());
}

void TripCancelledState::handle(Context& context, const std::string&) {
    std::cout << "Trip cancelled. Returning to Idle state.\n";
    context.setState(std::make_unique<IdleState>());
}

// Тестирование системы
int main() {
    Context context(std::make_unique<IdleState>());

    std::cout << "Current state: " << context.getStateName() << "\n";

    context.request("select_car");
    std::cout << "Current state: " << context.getStateName() << "\n";

    context.request("confirm_order");
    std::cout << "Current state: " << context.getStateName() << "\n";

    context.request("car_arrived");
    std::cout << "Current state: " << context.getStateName() << "\n";

    context.request("start_trip");
    std::cout << "Current state: " << context.getStateName() << "\n";

    context.request("complete_trip");
    std::cout << "Current state: " << context.getStateName() << "\n";

    return 0;
}
