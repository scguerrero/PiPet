/*
 * Start class specification file. This is the first "page" players will see.
 * Author(s): Sasha C. Guerrero
 * Created: 3/6/2026
 * Last Edited: 3/6/2026
 */

 #ifndef START_H
 #define START_H
 #include <QtWidgets>

 class Start : public QWidget {
 	Q_OBJECT
	public:
		explicit Start(QWidget *parent = nullptr);
		QPushButton *b_start; // either go to "Create" page or "Mode" page
	private:
		QVBoxLayout *layout; // vertically-arrange widgets
		QLabel *title; // title text
};

#endif
