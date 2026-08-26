import cv2

# Load gender model
gender_net = cv2.dnn.readNet(
    "gender_net.caffemodel",
    "deploy_gender.prototxt"
)

gender_list = ['Male', 'Female']

# Load face detection model
face_cascade = cv2.CascadeClassifier(
    cv2.data.haarcascades + 'haarcascade_frontalface_default.xml'
)

# Start webcam
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    for (x, y, w, h) in faces:
        # Draw rectangle
        cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

        # ✅ MUST be inside loop
        face = frame[y:y+h, x:x+w]
        if face.size == 0:
            continue

        blob = cv2.dnn.blobFromImage(
            face, 1.0, (227, 227),
            (78.4263377603, 87.7689143744, 114.895847746),
            swapRB=False
        )

        gender_net.setInput(blob)
        preds = gender_net.forward()
        gender = gender_list[preds[0].argmax()]

        cv2.putText(frame, gender, (x, y-10),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.9,
                    (0, 255, 0), 2)

    # ✅ Correct position
    cv2.imshow('Face Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
