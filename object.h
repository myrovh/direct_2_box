#ifndef OBJECT_H
#define OBJECT_H

class object
{
private:
	int reference_count;
	const char* filename;

protected:
	object();
	virtual ~object();
	void add_reference() {
		reference_count++;
	}
	void remove_reference() {
		reference_count--;
	}
	int get_reference_count(){
		return reference_count;
	}

public:
	const char* get_filename() {
		return filename;
	}

	virtual void release() = 0;
	virtual void render() = 0;

	friend class object_manager;
};

#endif