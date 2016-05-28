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

	bool isExtraPoints() const {
		return extraPoints;
	}

	void setExtraPoints(bool extraPoints) {
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

private:
	int id;
	int quantity;
	int hitId;
	bool extraPoints;
};

#endif /* FORMATION_H_ */

