/*
 * Formation.h
 *
 *  Created on: May 27, 2016
 *      Author: facundo
 */

#ifndef FORMATION_H_
#define FORMATION_H_

class Formation {
public:
	Formation();
	virtual ~Formation();

	int getExtraPoints() {
		return extraPoints;
	}

	void setExtraPoints(int extraPoints) {
		this->extraPoints = extraPoints;
	}

	int getHitId() const {
		return hitId;
	}

	void setHitId(int hitId) {
		this->hitId = hitId;
	}

	int getId() const {
		return id;
	}

	void setId(int id) {
		this->id = id;
	}

	int getQuantity() const {
		return quantity;
	}

	void setQuantity(int quantity) {
		this->quantity = quantity;
	}

	bool isBonus() const {
		return bonus;
	}

	void setBonus(bool bonus) {
		this->bonus = bonus;
	}

	bool isDestroyed() const {
		return destroyed;
	}

	void setDestroyed(bool destroyed) {
		this->destroyed = destroyed;
	}

private:
	int id;
	int quantity;
	int hitId;
	int extraPoints;
	bool bonus;
	bool destroyed;
};

#endif /* FORMATION_H_ */

